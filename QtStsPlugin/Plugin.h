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
#pragma once

#include "qtstsplugin_global.h"
#include <QObject>
#include <QString>
#include <QAbstractSocket>
#include "Train.h"
#include "Timetable.h"

class QTcpSocket;

namespace QtSts {

	class PluginCore;

	class QTSTSPLUGIN_EXPORT Plugin : public QObject
	{
		Q_OBJECT
		Q_PROPERTY(bool stsConnected READ isConnected WRITE setConnected NOTIFY stsConnected)

	public:
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
		void requestSimTime();
		void requestSignalBoxInfo();
		void requestHeat();
		void requestStitz();
		void requestTrainList();
		void requestTrainInfo(int trainId);
		void requestTimeTable(int trainId);
		void registerEvent(int trainId, QtSts::TrainEvent event);

	Q_SIGNALS:
		void stsConnected(bool connected);
		void signalConnected();
		void signalDisconnected();
		void statusMessageReceived(int code, const QString& text);
		void timeReceived(int offset, int rtt);
		void signalBoxInfoReceived(int simbuild, int aid, const QString& name);
		void heatReceived(int heat);
		void stitzReceived(int allgemein, int region);
		void trainListReceived(const QList<QPair<int, QString>>& trainList);
		void trainDetailsReceived(QtSts::Train train, QtSts::TrainEvent event);
		void timetableReceived(QtSts::Timetable timetable);
		void dataFromSts(const QByteArray& data);
		void dataToSts(const QByteArray& data);

	private Q_SLOTS:
		void on_readyRead();
		void on_socketStateChanged(QAbstractSocket::SocketState state);
		void on_coreRegistered();
		void sendToSocket(const QByteArray& data);

	private:
		PluginCore* m_core;
		QTcpSocket* m_socket;
		QString m_stsHostname;
		unsigned short m_stsPort;
		QAbstractSocket::NetworkLayerProtocol m_ipProtocol;
	};

}