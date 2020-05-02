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
#include "QtStsPluginTestGui.h"
#include "ui_QtStsPluginTestGui.h"

#include <QApplication>
#include <QScrollBar>
#include <QInputDialog>

#include "../QtStsPlugin/Plugin.h"
#include "DialogPlugin.h"
#include "DialogConnection.h"

QtStsPluginTestGui::QtStsPluginTestGui(QWidget *parent)
	: QMainWindow(parent)
	, ui(new Ui::QtStsPluginTestGuiClass)
	, m_plugin(nullptr)
{
	ui->setupUi(this);
	ui->actionDestroy->setEnabled(false);
	ui->actionSetConnection->setEnabled(false);
	ui->actionConnect->setEnabled(false);
	ui->menuRequest->setEnabled(false);
}

QtStsPluginTestGui::~QtStsPluginTestGui() = default;

void QtStsPluginTestGui::on_actionQuit_triggered()
{
	qApp->quit();
}

void QtStsPluginTestGui::on_actionInstantiate_triggered()
{
	Q_ASSERT(m_plugin == nullptr);

	DialogPlugin dialog;
	dialog.setName(QStringLiteral("QtStsPluginTestGui"));
	dialog.setAuthor(QStringLiteral("Jan Siebeneich"));
	dialog.setVersion(QStringLiteral("1.0"));
	dialog.setDescription(QStringLiteral("QtStsPlugin Library"));
	
	const int result = dialog.exec();

	if (result == QDialog::Accepted)
	{
		ui->actionInstantiate->setEnabled(false);
		m_plugin = new QtSts::Plugin(dialog.name(), dialog.author(), dialog.version(), dialog.description(), this);
		auto connection = QObject::connect(ui->actionConnect, SIGNAL(triggered(bool)), m_plugin, SLOT(setConnected(bool)));
		Q_ASSERT(connection);
		connection = QObject::connect(m_plugin, SIGNAL(dataToSts(const QByteArray&)), this, SLOT(communicationToSts(const QByteArray&)));
		Q_ASSERT(connection);
		connection = QObject::connect(m_plugin, SIGNAL(dataFromSts(const QByteArray&)), this, SLOT(communicationFromSts(const QByteArray&)));
		Q_ASSERT(connection);
		connection = QObject::connect(m_plugin, SIGNAL(signalConnected(bool)), this, SLOT(on_signalConnected(bool)));
		Q_ASSERT(connection);
		connection = QObject::connect(m_plugin, SIGNAL(statusMessageReceived(int, const QString&)), this, SLOT(on_statusMessageReceived(int, const QString&)));
		Q_ASSERT(connection);
		connection = QObject::connect(m_plugin, SIGNAL(timeReceived(int, int)), this, SLOT(on_timeReceived(int, int)));
		Q_ASSERT(connection);
		connection = QObject::connect(m_plugin, SIGNAL(signalBoxInfoReceived(int, int, const QString&)), this, SLOT(on_signalBoxInfoReceived(int, int, const QString&)));
		Q_ASSERT(connection);
		connection = QObject::connect(m_plugin, SIGNAL(heatReceived(int)), this, SLOT(on_heatReceived(int)));
		Q_ASSERT(connection);
		connection = QObject::connect(m_plugin, SIGNAL(stitzReceived(int, int)), this, SLOT(on_stitzReceived(int, int)));
		Q_ASSERT(connection);
		connection = QObject::connect(m_plugin, SIGNAL(trainListReceived(const QList<QPair<int, QString>>&)), this, SLOT(on_trainListReceived(const QList<QPair<int, QString>>&)));
		Q_ASSERT(connection);
		connection = QObject::connect(m_plugin, SIGNAL(trainDetailsReceived(int, const QString&, const QString&, const QString&, const QString&, const QString&, int, bool, bool)),
			this, SLOT(on_trainDetailsReceived(int, const QString&, const QString&, const QString&, const QString&, const QString&, int, bool, bool)));
		Q_ASSERT(connection);
		connection = QObject::connect(m_plugin, SIGNAL(incomingTrainReceived(int, const QString&, const QString&, const QString&, const QString&, const QString&, int, bool, bool)),
			this, SLOT(on_incomingTrainReceived(int, const QString&, const QString&, const QString&, const QString&, const QString&, int, bool, bool)));
		Q_ASSERT(connection);
		connection = QObject::connect(m_plugin, SIGNAL(outgoingTrainReceived(int, const QString&, const QString&, const QString&, const QString&, const QString&, int, bool, bool)),
			this, SLOT(on_outgoingTrainReceived(int, const QString&, const QString&, const QString&, const QString&, const QString&, int, bool, bool)));
		Q_ASSERT(connection);
		connection = QObject::connect(m_plugin, SIGNAL(arrivingTrainReceived(int, const QString&, const QString&, const QString&, const QString&, const QString&, int, bool, bool)),
			this, SLOT(on_arrivingTrainReceived(int, const QString&, const QString&, const QString&, const QString&, const QString&, int, bool, bool)));
		Q_ASSERT(connection);
		connection = QObject::connect(m_plugin, SIGNAL(departingTrainReceived(int, const QString&, const QString&, const QString&, const QString&, const QString&, int, bool, bool)),
			this, SLOT(on_departingTrainReceived(int, const QString&, const QString&, const QString&, const QString&, const QString&, int, bool, bool)));
		Q_ASSERT(connection);

		ui->actionDestroy->setEnabled(true);
		ui->actionSetConnection->setEnabled(true);
		ui->actionConnect->setEnabled(true);
		ui->menuRequest->setEnabled(true);
	}
}

void QtStsPluginTestGui::on_actionDestroy_triggered()
{
	Q_ASSERT(m_plugin != nullptr);

	delete m_plugin;
	m_plugin = nullptr;

	ui->actionInstantiate->setEnabled(true); 
	ui->actionDestroy->setEnabled(false);
	ui->actionSetConnection->setEnabled(false);
	ui->actionConnect->setEnabled(false);
	ui->actionConnect->setChecked(false);
	ui->menuRequest->setEnabled(false);
}

void QtStsPluginTestGui::on_actionSetConnection_triggered()
{
	Q_ASSERT(m_plugin != nullptr);

	DialogConnection dialog;
	dialog.setHostname(m_plugin->stsHostname());
	dialog.setPort(m_plugin->stsPort());
	dialog.setProtocol(m_plugin->ipProtocol());

	const int result = dialog.exec();

	if (result == QDialog::Accepted)
	{
		m_plugin->setStsHostname(dialog.hostname());
		m_plugin->setStsPort(dialog.port());
		m_plugin->setIpProtocol(dialog.protocol());
	}
}

void QtStsPluginTestGui::on_actionSimTime_triggered()
{
	Q_ASSERT(m_plugin != nullptr);

	m_plugin->requestSimTime();
}

void QtStsPluginTestGui::on_actionSignalBoxInfo_triggered()
{
	Q_ASSERT(m_plugin != nullptr);

	m_plugin->requestSignalBoxInfo();
}

void QtStsPluginTestGui::on_actionHeat_triggered()
{
	m_plugin->requestHeat();
}

void QtStsPluginTestGui::on_actionStitz_triggered()
{
	m_plugin->requestStitz();
}

void QtStsPluginTestGui::on_actionTrainList_triggered()
{
	m_plugin->requestTrainList();
}

void QtStsPluginTestGui::on_actionTrainInfo_triggered()
{
	bool ok = false;
	const int trainId = QInputDialog::getInt(this, tr("Request Train Info"), tr("Train ID"), 0, -2147483647, 2147483647, 1, &ok);
	if (ok)
	{
		m_plugin->requestTrainInfo(trainId);
	}
}

void QtStsPluginTestGui::communicationFromSts(const QByteArray& data)
{
	addToCommunicationLog(data, false, true);
}

void QtStsPluginTestGui::communicationToSts(const QByteArray& data)
{
	addToCommunicationLog(data, true, true);
}

void QtStsPluginTestGui::on_signalConnected(bool connected)
{
	ui->logText->append(tr("signalConnected: connected=%1").arg(connected));
}

void QtStsPluginTestGui::on_statusMessageReceived(int code, const QString& text)
{
	ui->logText->append(tr("statusMessageReceived: code=%1, text=\"%2\"").arg(code).arg(text));
}

void QtStsPluginTestGui::on_timeReceived(int offset, int rtt)
{
	ui->logText->append(tr("timeReceived: offset=%1, rtt=%2").arg(offset).arg(rtt));
}

void QtStsPluginTestGui::on_signalBoxInfoReceived(int simbuild, int aid, const QString& name)
{
	ui->logText->append(tr("signalBoxInfoReceived: simbuild=%1, aid=%2, name=\"%3\"").arg(simbuild).arg(aid).arg(name));
}

void QtStsPluginTestGui::on_heatReceived(int heat)
{
	ui->logText->append(tr("heatReceived: heat=%1").arg(heat));
}

void QtStsPluginTestGui::on_stitzReceived(int allgemein, int region)
{
	ui->logText->append(tr("stitzReceived: allgemein=%1, region=%2").arg(allgemein).arg(region));
}

void QtStsPluginTestGui::on_trainListReceived(const QList<QPair<int, QString>>& trainList)
{
	QString log(tr("trainListReceived: trainList="));
	for (auto train : trainList)
	{
		log.append(QStringLiteral("\n\t%1 / %2").arg(train.first).arg(train.second));
	}
	ui->logText->append(log);
}

namespace {
	void addLogText(QString& log, int trainId, const QString& name, const QString& from, const QString& to, const QString& track, const QString& plannedTrack, int delay, bool onTrack, bool visible)
	{
		log.append(QStringLiteral("\n\ttrainId=%1").arg(trainId));
		log.append(QStringLiteral("\n\tname=\"%1\"").arg(name));
		log.append(QStringLiteral("\n\tfrom=\"%1\"\n\tto=\"%2\"").arg(from).arg(to));
		log.append(QStringLiteral("\n\ttrack=\"%1\"\n\tplannedTrack=\"%2\"").arg(track).arg(plannedTrack));
		log.append(QStringLiteral("\n\tdelay=%1").arg(delay));
		log.append(QStringLiteral("\n\tonTrack=%1\n\tvisible=%2").arg(onTrack).arg(visible));
	}
}

void QtStsPluginTestGui::on_trainDetailsReceived(int trainId, const QString& name, const QString& from, const QString& to, const QString& track, const QString& plannedTrack, int delay, bool onTrack, bool visible)
{
	QString log(tr("trainDetailsReceived:"));
	addLogText(log, trainId, name, from, to, track, plannedTrack, delay, onTrack, visible);
	ui->logText->append(log);

}

void QtStsPluginTestGui::on_incomingTrainReceived(int trainId, const QString& name, const QString& from, const QString& to, const QString& track, const QString& plannedTrack, int delay, bool onTrack, bool visible)
{
	QString log(tr("incomingTrainReceived:"));
	addLogText(log, trainId, name, from, to, track, plannedTrack, delay, onTrack, visible);
	ui->logText->append(log);
}

void QtStsPluginTestGui::on_outgoingTrainReceived(int trainId, const QString& name, const QString& from, const QString& to, const QString& track, const QString& plannedTrack, int delay, bool onTrack, bool visible)
{
	QString log(tr("outgoingTrainReceived:"));
	addLogText(log, trainId, name, from, to, track, plannedTrack, delay, onTrack, visible);
	ui->logText->append(log);
}

void QtStsPluginTestGui::on_arrivingTrainReceived(int trainId, const QString& name, const QString& from, const QString& to, const QString& track, const QString& plannedTrack, int delay, bool onTrack, bool visible)
{
	QString log(tr("arrivingTrainReceived:"));
	addLogText(log, trainId, name, from, to, track, plannedTrack, delay, onTrack, visible);
	ui->logText->append(log);
}

void QtStsPluginTestGui::on_departingTrainReceived(int trainId, const QString& name, const QString& from, const QString& to, const QString& track, const QString& plannedTrack, int delay, bool onTrack, bool visible)
{
	QString log(tr("departingTrainReceived:"));
	addLogText(log, trainId, name, from, to, track, plannedTrack, delay, onTrack, visible);
	ui->logText->append(log);
}

void QtStsPluginTestGui::addToCommunicationLog(const QByteArray& data, bool outgoing, bool scrollToEnd)
{
	QString sHtml(QStringLiteral("<p style=\"color:"));
	sHtml.append(outgoing ? QStringLiteral("blue") : QStringLiteral("red"));
	sHtml.append(QStringLiteral("\">")).append(QString(data).toHtmlEscaped());
	sHtml.append(QStringLiteral("</p><br />"));
	
	auto cursor = ui->communicationText->textCursor();
	cursor.movePosition(QTextCursor::End);
	cursor.insertHtml(sHtml);

	if (scrollToEnd)
	{
		auto scrollBar = ui->communicationText->verticalScrollBar();
		scrollBar->setValue(scrollBar->maximum());
	}
}
