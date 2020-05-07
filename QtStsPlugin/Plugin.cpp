/*****************************************************************************
QtStsPlugin - Qt-based library for using StellwerkSim's plugin interface
Copyright (C) 2020  Jan Siebeneich

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published
by the Free Software Foundation, version 3 of the License.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.
*****************************************************************************/
#include "Plugin.h"

#include <QTcpSocket>
#include <QByteArray>
#include <QMetaObject>
#include <QMetaMethod>
#include "PluginCore.h"

QtSts::Plugin::Plugin(const QString& pluginName,
	const QString& pluginAuthor,
	const QString& pluginVersion,
	const QString& pluginText,
	QObject* parent)
	: QObject(parent)
	, m_core(nullptr)
	, m_socket(nullptr)
	, m_stsHostname("localhost")
	, m_stsPort(3691)
	, m_ipProtocol(QAbstractSocket::AnyIPProtocol)
{
	m_socket = new QTcpSocket(this);
	QObject::connect(m_socket, SIGNAL(readyRead()), this, SLOT(on_readyRead()));
	QObject::connect(m_socket, SIGNAL(socketStateChanged(QAbstractSocket::SocketState)), this, SLOT(on_socketStateChanged(QAbstractSocket::SocketState)));

	m_core = new PluginCore(pluginName, pluginAuthor, pluginVersion, pluginText, this);
	QObject::connect(m_core, SIGNAL(sendToSts(const QByteArray&)), this, SLOT(sendToSocket(const QByteArray&)));

	auto metaCore = m_core->metaObject();
	auto metaThis = metaObject();

	// Connect signals from PluginCore to signals from Plugin if signature is identical
	for (int i = metaCore->methodOffset(); i < metaCore->methodCount(); ++i)
	{
		auto coreMethod = metaCore->method(i);
		if (coreMethod.methodType() == QMetaMethod::Signal)
		{
			int signalIndex = metaThis->indexOfSignal(coreMethod.methodSignature());
			if (signalIndex >= 0)
			{
				auto connection = QObject::connect(m_core, coreMethod, this, metaThis->method(signalIndex));
				Q_ASSERT(connection);
			}
		}
	}
}

QtSts::Plugin::~Plugin()
{
	m_core = nullptr;
	m_socket = nullptr;
}

bool QtSts::Plugin::isConnected() const
{
	return m_core->isRegistered();
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

void QtSts::Plugin::requestSimTime()
{
	m_core->requestSimTime();
}

void QtSts::Plugin::requestSignalBoxInfo()
{
	m_core->requestSignalBoxInfo();
}

void QtSts::Plugin::requestHeat()
{
	m_core->requestHeat();
}

void QtSts::Plugin::requestStitz()
{
	m_core->requestStitz();
}

void QtSts::Plugin::requestTrainList()
{
	m_core->requestTrainList();
}

void QtSts::Plugin::requestTrainInfo(int trainId)
{
	m_core->requestTrainInfo(trainId);
}

void QtSts::Plugin::requestTimeTable(int trainId)
{
	m_core->requestTimeTable(trainId);
}

void QtSts::Plugin::registerEvent(int trainId, QtSts::TrainEvent event)
{
	m_core->registerEvent(trainId, event);
}

void QtSts::Plugin::on_readyRead()
{
	const QByteArray buffer(m_socket->readAll());
	Q_EMIT dataFromSts(buffer);
	m_core->receivedFromSts(buffer);
}

void QtSts::Plugin::on_socketStateChanged(QAbstractSocket::SocketState state)
{
	switch (state)
	{
	case QAbstractSocket::UnconnectedState:
		if (m_core->isStreamActive())
		{
			m_core->receivedFromSts(QByteArrayLiteral("</sts:stream>\n"));
		}
		break;
	case QAbstractSocket::ConnectedState:
		Q_ASSERT(m_core->isStreamActive() == false);
		m_core->receivedFromSts(QByteArrayLiteral("<sts:stream>\n"));
		break;
	default:
		break;
	}
}

void QtSts::Plugin::sendToSocket(const QByteArray& data)
{
	Q_EMIT dataToSts(data);
	m_socket->write(data);
}
