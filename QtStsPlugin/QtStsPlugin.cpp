#include "QtStsPlugin.h"

QtSts::Plugin::Plugin(const QString& sName,
	const QString& sAuthor,
	const QString& sVersion,
	const QString& sText,
	QObject* parent)
	: QObject(parent)
{
}

QtSts::Plugin::~Plugin() = default;
