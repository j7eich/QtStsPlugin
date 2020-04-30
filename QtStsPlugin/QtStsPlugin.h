#pragma once

#include "qtstsplugin_global.h"
#include <QObject>
#include <QString>
#include "TrainEventFlags.h"

namespace QtSts {

	class QTSTSPLUGIN_EXPORT Plugin : public QObject
	{
		Q_OBJECT

	public:
		Q_FLAG(TrainEvents)

		Plugin(const QString& pluginName,
			const QString& pluginAuthor,
			const QString& pluginVersion,
			const QString& pluginDescription,
			QObject* parent = Q_NULLPTR);
		~Plugin() override;
	};

}