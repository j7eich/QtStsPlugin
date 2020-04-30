#include "Plugin.h"

#include <QTcpSocket>
#include <QByteArray>
#include "PluginCore.h"

QtSts::Plugin::Plugin(const QString& pluginName,
	const QString& pluginAuthor,
	const QString& pluginVersion,
	const QString& pluginText,
	QObject* parent)
	: QObject(parent)
	, m_core(nullptr)
	, m_socket(nullptr)
	, m_stsHostname()
	, m_stsPort(3691)
	, m_ipProtocol(QAbstractSocket::AnyIPProtocol)
{
	m_socket = new QTcpSocket(this);
	QObject::connect(m_socket, SIGNAL(readyRead()), this, SLOT(on_readyRead()));
	QObject::connect(m_socket, SIGNAL(socketStateChanged(QAbstractSocket::SocketState)), this, SLOT(on_socketStateChanged(QAbstractSocket::SocketState)));

	m_core = new PluginCore(pluginName, pluginAuthor, pluginVersion, pluginText, this);
}

QtSts::Plugin::~Plugin()
{
	m_core = nullptr;
	m_socket = nullptr;
}

bool QtSts::Plugin::isConnected() const
{
	return false;
}

void QtSts::Plugin::setStsHostname(const QString& hostname)
{
	m_stsHostname = hostname;
}

void QtSts::Plugin::setStsPort(unsigned short port)
{
	m_stsPort = port;
}

void QtSts::Plugin::setIpProtocol(QAbstractSocket::NetworkLayerProtocol protocol)
{
	m_ipProtocol = protocol;
}

void QtSts::Plugin::setConnected(bool connect)
{
	if (connect)
	{
		m_socket->connectToHost(m_stsHostname, m_stsPort,
			QIODevice::ReadWrite, m_ipProtocol);
	}
	else
	{
		m_socket->disconnectFromHost();
	}
}

void QtSts::Plugin::on_readyRead()
{
	const QByteArray buffer(m_socket->readAll());
	m_core->parseData(buffer);
}

void QtSts::Plugin::on_socketStateChanged(QAbstractSocket::SocketState state)
{
	switch (state)
	{
	case QAbstractSocket::UnconnectedState:
		if (m_core->isConnected())
		{
			m_core->parseData(QByteArrayLiteral("</sts:stream>\n"));
		}
		break;
	case QAbstractSocket::ConnectedState:
		m_core->parseData(QByteArrayLiteral("<sts:stream>\n"));
		break;
	default:
		break;
	}
}
