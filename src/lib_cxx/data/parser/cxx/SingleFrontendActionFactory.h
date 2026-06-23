#ifndef SINGLE_FRONTEND_ACTION_FACTORY
#define SINGLE_FRONTEND_ACTION_FACTORY

#include <memory>

#include <clang/Tooling/Tooling.h>

class SingleFrontendActionFactory: public clang::tooling::FrontendActionFactory
{
public:
	SingleFrontendActionFactory(clang::FrontendAction* action);
	std::unique_ptr<clang::FrontendAction> create() override;

private:
	std::unique_ptr<clang::FrontendAction> m_action;
};

#endif	  // SINGLE_FRONTEND_ACTION_FACTORY
