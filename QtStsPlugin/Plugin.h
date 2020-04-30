#pragma once

#include "qtstsplugin_global.h"
#include <QObject>
#include <QString>
#include <QAbstractSocket>
#include "TrainEventFlags.h"

class QTcpSocket;

namespace QtSts {

	class PluginCore;

	class QTSTSPLUGIN_EXPORT Plugin : public QObject
	{
		Q_OBJECT
		Q_PROPERTY(bool stsConnected READ isConnected WRITE setConnected NOTIFY signalConnected)

	public:
		Q_FLAG(TrainEvents)

		Plugin(const QString& pluginName,
			const QString& pluginAuthor,
			const QString& pluginVersion,
			const QString& pluginDescription,
			QObject* parent = nullptr);
		~Plugin() override;

		const QString& stsHostname() const { return m_stsHostname; }
		constexpr unsigned short stsPort() const { return m_stsPort; }
		constexpr QAbstractSocket::NetworkLayerProtocol ipProtocol() const { return m_ipProtocol; }
		bool isConnected() const;

	public Q_SLOTS:
		void setStsHostname(const QString& hostname);
		void setStsPort(unsigned short port);
		void setIpProtocol(QAbstractSocket::NetworkLayerProtocol protocol);
		void setConnected(bool connect);
		void stsConnect() { setConnected(true); }
		void stsDisconnect() { setConnected(false); }

	Q_SIGNALS:
		void signalConnected(bool connected);

	private Q_SLOTS:
		void on_readyRead();
		void on_socketStateChanged(QAbstractSocket::SocketState state);

	private:
		PluginCore* m_core;
		QTcpSocket* m_socket;
		QString m_stsHostname;
		unsigned short m_stsPort;
		QAbstractSocket::NetworkLayerProtocol m_ipProtocol;
	};

}