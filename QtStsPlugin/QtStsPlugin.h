#pragma once

#include "qtstsplugin_global.h"
#include <QObject>
#include <QString>

class QTSTSPLUGIN_EXPORT QtStsPlugin : public QObject
{
	Q_OBJECT

public:
	enum TrainEvent
	{
		NONE      = 0b0000,
		INBOUND   = 0b0001,
		ARRIVAL   = 0b0010,
		DEPARTURE = 0b0100,
		OUTBOUND  = 0b1000
	};
	Q_DECLARE_FLAGS(TrainEvents, TrainEvent)
	Q_FLAG(TrainEvents)

	QtStsPlugin(const QString& pluginName,
		const QString& pluginAuthor,
		const QString& pluginVersion,
		const QString& pluginDescription,
		QObject* parent = Q_NULLPTR);
	~QtStsPlugin() override;
};
