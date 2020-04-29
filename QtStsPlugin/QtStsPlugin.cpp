#include "QtStsPlugin.h"

QtStsPlugin::QtStsPlugin(const QString& sName,
	const QString& sAuthor,
	const QString& sVersion,
	const QString& sText,
	QObject* parent)
	: QObject(parent)
{
}

QtStsPlugin::~QtStsPlugin() = default;
