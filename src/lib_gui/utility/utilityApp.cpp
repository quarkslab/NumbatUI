#include "utilityApp.h"

#include <chrono>
#include <mutex>
#include <set>
#include <thread>

#include <boost/asio/buffer.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/asio/read.hpp>
// This file uses the Boost.Process v1 API (child, async_pipe, the redirection
// operators, this_process::environment, ...). The build defines
// BOOST_PROCESS_VERSION=1, which makes the boost::process::v1 namespace inline,
// so the unqualified boost::process:: / boost::this_process:: names resolve to
// v1 regardless of Boost version. We only need to make sure the right headers
// are found: Boost >= 1.86 ships the v1 facilities under boost/process/v1/*,
// while older Boost uses the legacy boost/process/* paths. Include whichever
// exists.
#if __has_include(<boost/process/v1/child.hpp>)
#  include <boost/process/v1/args.hpp>
#  include <boost/process/v1/async_pipe.hpp>
#  include <boost/process/v1/child.hpp>
#  include <boost/process/v1/environment.hpp>
#  include <boost/process/v1/io.hpp>
#  include <boost/process/v1/search_path.hpp>
#  include <boost/process/v1/start_dir.hpp>
#elif __has_include(<boost/process/v1.hpp>)
#  include <boost/process/v1.hpp>
#else
#  include <boost/process.hpp>
#  include <boost/process/async_pipe.hpp>
#  include <boost/process/child.hpp>
#  include <boost/process/environment.hpp>
#  include <boost/process/io.hpp>
#  include <boost/process/search_path.hpp>
#  include <boost/process/start_dir.hpp>
#endif

#include <QThread>

#include "ScopedFunctor.h"
#include "logging.h"
#include "utilityString.h"

namespace utility
{
std::mutex s_runningProcessesMutex;
std::set<std::shared_ptr<boost::process::child>> s_runningProcesses;
}	 // namespace utility

std::string utility::getDocumentationLink()
{
	return "https://github.com/quarkslab/Numbat/blob/master/DOCUMENTATION.md";
}

std::wstring utility::searchPath(const std::wstring& bin, bool& ok)
{
	ok = false;
	std::wstring r = boost::process::search_path(bin).generic_wstring();
	if (!r.empty())
	{
		ok = true;
		return r;
	}
	return bin;
}

std::wstring utility::searchPath(const std::wstring& bin)
{
	bool ok;
	return searchPath(bin, ok);
}

namespace
{
template <typename Rep, typename Period>
bool safely_wait_for(boost::process::child& process, const std::chrono::duration<Rep, Period>& rel_time)
{
	// Poll child::running() until the process exits or the timeout elapses,
	// instead of boost::process::child::wait_for, which is deprecated and
	// documented as unreliable. running() reaps the process once it has exited,
	// so the exit code is available afterwards.
	//
	// This also handles the edge case the previous implementation guarded
	// against: an already-exited process returns immediately rather than
	// blocking for the whole timeout.
	const auto deadline = std::chrono::steady_clock::now() + rel_time;
	while (process.running())
	{
		if (std::chrono::steady_clock::now() >= deadline)
		{
			return false;	// still running when the timeout elapsed
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}
	return true;	// the process exited
}
}	 // namespace

utility::ProcessOutput utility::executeProcess(
	const std::wstring& command,
	const std::vector<std::wstring>& arguments,
	const FilePath& workingDirectory,
	const bool waitUntilNoOutput,
	const int timeout,
	bool logProcessOutput)
{
	std::string output = "";
	int exitCode = 255;
	try
	{
		boost::asio::io_context ios;
		boost::process::async_pipe ap(ios);

		std::shared_ptr<boost::process::child> process;

		// The command and arguments below are wchar_t (searchPath returns
		// std::wstring, arguments is std::vector<std::wstring>). Boost.Process
		// requires the environment to use the same character width as the rest
		// of the child initializers, so use a wide environment here. Mixing a
		// narrow (char) environment with a wide command fails to instantiate
		// under Clang 19's stricter template checking.
		boost::process::wenvironment env = boost::this_process::wenvironment();
		std::vector<std::wstring> previousPath = env[L"PATH"].to_vector();
		env[L"PATH"] = {L"/opt/local/bin", L"/usr/local/bin", L"$HOME/bin"};
		for (const std::wstring& entry: previousPath)
		{
			env[L"PATH"].append(entry);
		}

		if (workingDirectory.empty())
		{
			process = std::make_shared<boost::process::child>(
				searchPath(command),
				boost::process::args(arguments),
				env,
				boost::process::std_in.close(),
				(boost::process::std_out & boost::process::std_err) > ap);
		}
		else
		{
			process = std::make_shared<boost::process::child>(
				searchPath(command),
				boost::process::args(arguments),
				boost::process::start_dir(workingDirectory.wstr()),
				env,
				boost::process::std_in.close(),
				(boost::process::std_out & boost::process::std_err) > ap);
		}

		{
			std::lock_guard<std::mutex> lock(s_runningProcessesMutex);
			s_runningProcesses.insert(process);
		}

		ScopedFunctor remover(
			[process]()
			{
				std::lock_guard<std::mutex> lock(s_runningProcessesMutex);
				s_runningProcesses.erase(process);
			});

		bool outputReceived = false;
		std::vector<char> buf(128);
		auto stdOutBuffer = boost::asio::buffer(buf);
		std::string logBuffer;

		std::function<void(const boost::system::error_code& ec, std::size_t n)> onStdOut =
			[&output, &buf, &stdOutBuffer, &ap, &onStdOut, &outputReceived, &logBuffer, logProcessOutput](
				const boost::system::error_code& ec, std::size_t size)
		{
			std::string text;
			text.reserve(size);
			text.insert(text.end(), buf.begin(), buf.begin() + size);

			if (!text.empty())
			{
				outputReceived = true;
			}

			output += text;
			if (logProcessOutput)
			{
				logBuffer += text;
				const bool isEndOfLine = (logBuffer.back() == '\n');
				const std::vector<std::string> lines = utility::splitToVector(logBuffer, "\n");
				for (size_t i = 0; i < lines.size() - (isEndOfLine ? 0 : 1); i++)
				{
					LOG_INFO_BARE("Process output: " + lines[i]);
				}
				if (isEndOfLine)
				{
					logBuffer.clear();
				}
				else
				{
					logBuffer = lines.back();
				}
			}
			if (!ec)
			{
				boost::asio::async_read(ap, stdOutBuffer, onStdOut);
			}
		};

		boost::asio::async_read(ap, stdOutBuffer, onStdOut);
		ios.run();

		if (timeout > 0)
		{
			if (waitUntilNoOutput)
			{
				while (!safely_wait_for(*process, std::chrono::milliseconds(timeout)))
				{
					if (!outputReceived)
					{
						LOG_WARNING(
							"Canceling process because it did not generate any output during the "
							"last " +
							std::to_string(timeout / 1000) + " seconds.");
						process->terminate();
						break;
					}
					outputReceived = false;
				}
			}
			else
			{
				if (!safely_wait_for(*process, std::chrono::milliseconds(timeout)))
				{
					LOG_WARNING(
						"Canceling process because it timed out after " +
						std::to_string(timeout / 1000) + " seconds.");
					process->terminate();
				}
			}
		}
		else
		{
			process->wait();
		}

		if (logProcessOutput)
		{
			for (const std::string& line: utility::splitToVector(logBuffer, "\n"))
			{
				LOG_INFO_BARE("Process output: " + line);
			}
		}

		exitCode = process->exit_code();
	}
	catch (const boost::process::process_error& e)
	{
		ProcessOutput ret;
		ret.error = utility::decodeFromUtf8(e.code().message());
		ret.exitCode = e.code().value();
		LOG_ERROR_BARE(L"Process error: " + ret.error);

		return ret;
	}

	ProcessOutput ret;
	ret.output = utility::trim(utility::decodeFromUtf8(output));
	ret.exitCode = exitCode;
	return ret;
}

void utility::killRunningProcesses()
{
	std::lock_guard<std::mutex> lock(s_runningProcessesMutex);
	for (std::shared_ptr<boost::process::child> process: s_runningProcesses)
	{
		process->terminate();
	}
}

int utility::getIdealThreadCount()
{
	int threadCount = QThread::idealThreadCount();
	if (getOsType() == OS_WINDOWS)
	{
		threadCount -= 1;
	}
	return std::max(1, threadCount);
}

std::string utility::getOsTypeString()
{
	// WARNING: Don't change these string. The server API relies on them.
	switch (utility::getOsType())
	{
	case OS_WINDOWS:
		return "windows";
	case OS_MAC:
		return "macOS";
	case OS_LINUX:
		return "linux";
	default:
		break;
	}
	return "unknown";
}
