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

#include <QMainWindow>
#include <memory>

namespace Ui {
	class QtStsPluginTestGuiClass;
}

namespace QtSts {
	class Plugin;
}

class QtStsPluginTestGui : public QMainWindow
{
	Q_OBJECT

public:
	QtStsPluginTestGui(QWidget* parent = nullptr);
	~QtStsPluginTestGui() override;

public Q_SLOTS:
	void on_actionQuit_triggered();
	void on_actionInstantiate_triggered();
	void on_actionDestroy_triggered();
	void on_actionSetConnection_triggered();
	void on_actionSimTime_triggered();
	void on_actionSignalBoxInfo_triggered();
	void on_actionHeat_triggered();
	void on_actionStitz_triggered();
	void on_actionTrainList_triggered();
	void on_actionTrainInfo_triggered();
	void communicationFromSts(const QByteArray& data);
	void communicationToSts(const QByteArray& data);

	void on_signalConnected(bool connected);
	void on_statusMessageReceived(int code, const QString& text);
	void on_timeReceived(int offset, int rtt);
	void on_signalBoxInfoReceived(int simbuild, int aid, const QString& name);
	void on_heatReceived(int heat);
	void on_stitzReceived(int allgemein, int region);
	void on_trainListReceived(const QList<QPair<int, QString>>& trainList);
	void on_trainDetailsReceived(int trainId, const QString& name,
		const QString& from, const QString& to,
		const QString& track, const QString& plannedTrack,
		int delay, bool onTrack, bool visible);
	void on_incomingTrainReceived(int trainId, const QString& name,
		const QString& from, const QString& to,
		const QString& track, const QString& plannedTrack,
		int delay, bool onTrack, bool visible);
	void on_outgoingTrainReceived(int trainId, const QString& name,
		const QString& from, const QString& to,
		const QString& track, const QString& plannedTrack,
		int delay, bool onTrack, bool visible);
	void on_arrivingTrainReceived(int trainId, const QString& name,
		const QString& from, const QString& to,
		const QString& track, const QString& plannedTrack,
		int delay, bool onTrack, bool visible);
	void on_departingTrainReceived(int trainId, const QString& name,
		const QString& from, const QString& to,
		const QString& track, const QString& plannedTrack,
		int delay, bool onTrack, bool visible);

private:
	void addToCommunicationLog(const QByteArray& data, bool outgoing, bool scrollToEnd);

	std::unique_ptr<Ui::QtStsPluginTestGuiClass> ui;
	QtSts::Plugin* m_plugin;
};
