#pragma once

#include "qtstsplugin_global.h"
#include <QObject>
#include <QString>

namespace QtSts {

	class QTSTSPLUGIN_EXPORT PluginCore : public QObject
	{
		Q_OBJECT

	public:
		PluginCore(QObject* parent);
		~PluginCore() override;
	};

}