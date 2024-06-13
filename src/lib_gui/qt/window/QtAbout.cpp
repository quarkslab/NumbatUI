#include "QtAbout.h"

#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>

#include "QtDeviceScaledPixmap.h"
#include "ResourcePaths.h"
#include "SqliteIndexStorage.h"
#include "Version.h"
#include "utilityApp.h"
#include "utilityQt.h"

QtAbout::QtAbout(QWidget* parent): QtWindow(false, parent) {}

QSize QtAbout::sizeHint() const
{
	return QSize(450, 480);
}

void QtAbout::setupAbout()
{
	setStyleSheet(
		utility::getStyleSheet(ResourcePaths::getGuiDirectoryPath().concatenate(L"about/about.css"))
			.c_str());

	QVBoxLayout* windowLayout = new QVBoxLayout();
	windowLayout->setContentsMargins(10, 10, 10, 0);
	windowLayout->setSpacing(1);
	m_content->setLayout(windowLayout);

	{
		QtDeviceScaledPixmap sourcetrailLogo(QString::fromStdWString(
			ResourcePaths::getGuiDirectoryPath().wstr() + L"window/numbatui.png"));
		sourcetrailLogo.scaleToHeight(150);
		QLabel* sourcetrailLogoLabel = new QLabel(this);
		sourcetrailLogoLabel->setPixmap(sourcetrailLogo.pixmap());
		sourcetrailLogoLabel->resize(
			static_cast<int>(sourcetrailLogo.width()), static_cast<int>(sourcetrailLogo.height()));
		windowLayout->addWidget(
			sourcetrailLogoLabel, 0, Qt::Alignment(Qt::AlignmentFlag::AlignHCenter));
	}

	windowLayout->addSpacing(10);

	{
		QLabel* versionLabel = new QLabel(
			("Version " + Version::getApplicationVersion().toDisplayString() + " - " +
			 std::string(
				 utility::getApplicationArchitectureType() == APPLICATION_ARCHITECTURE_X86_32
					 ? "32"
					 : "64") +
			 " bit")
				.c_str(),
			this);
		windowLayout->addWidget(versionLabel, 0, Qt::Alignment(Qt::AlignmentFlag::AlignHCenter));
	}

	{
		QLabel* dbVersionLabel = new QLabel(
			"Database Version " + QString::number(SqliteIndexStorage::getStorageVersion()), this);
		windowLayout->addWidget(dbVersionLabel, 0, Qt::Alignment(Qt::AlignmentFlag::AlignHCenter));
	}

	windowLayout->addStretch();

	{
		QHBoxLayout* layoutHorz1 = new QHBoxLayout();
		windowLayout->addLayout(layoutHorz1);

		layoutHorz1->addStretch();

		QLabel* developerLabel = new QLabel(
			QString::fromStdString("<br /><br />"
								   "Developed by Quarkslab<br />"));

		developerLabel->setObjectName(QStringLiteral("small"));
		layoutHorz1->addWidget(developerLabel);

		layoutHorz1->addStretch();
	}

	windowLayout->addStretch();

	{
		QLabel* acknowledgementsLabel = new QLabel(QString::fromStdString(
			"<b>Acknowledgements:</b><br />"
			"NumbatUI is forked from <b><a href=\"https://github.com/CoatiSoftware/Sourcetrail\" "
			"style=\"color:white;\">Sourcetrail</a></b>, originally developed by:<br /><br />"
			"Manuel Dobusch<br />"
			"Eberhard Gräther<br />"
			"Malte Langkabel<br />"
			"Viktoria Pfausler<br />"
			"Andreas Stallinger<br />"));
		acknowledgementsLabel->setObjectName(QStringLiteral("small"));
		acknowledgementsLabel->setWordWrap(true);
		acknowledgementsLabel->setOpenExternalLinks(true);
		windowLayout->addWidget(acknowledgementsLabel);
		windowLayout->addSpacing(10);
	}

	{
		QLabel* webLabel = new QLabel(
			"<b>Repository: <a href=\"https://github.com/quarkslab/Numbat-UI\" "
			"style=\"color: "
			"white;\">https://github.com/quarkslab/Numbat-UI</a></b>",
			this);
		webLabel->setObjectName(QStringLiteral("small"));
		webLabel->setOpenExternalLinks(true);
		windowLayout->addWidget(webLabel);
		windowLayout->addSpacing(10);
	}
}
