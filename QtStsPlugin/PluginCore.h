#pragma once

#include "qtstsplugin_global.h"
#include <QObject>
#include <QString>
#include <memory>

class QXmlStreamReader;
class QByteArray;

namespace QtSts {

	class QTSTSPLUGIN_EXPORT PluginCore : public QObject
	{
		Q_OBJECT

	public:
		PluginCore(const QString& pluginName,
			const QString& pluginAuthor,
			const QString& pluginVersion,
			const QString& pluginDescription,
			QObject* parent = nullptr);
		~PluginCore() override;

		constexpr bool isConnected() const { return m_connected; }

	public Q_SLOTS:
		void parseData(const QByteArray& data);

	Q_SIGNALS:
		void communicationReady();
		void communicationStopped();

	private:
		void handleStartElement();
		void handleEndElement();

		std::unique_ptr<QXmlStreamReader> m_xmlReader;
		QString m_pluginName;
		QString m_pluginAuthor;
		QString m_pluginVersion;
		QString m_pluginDescription;
		bool m_connected;
		int m_inStatus;
	};

}