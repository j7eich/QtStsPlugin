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
#include "DialogEvent.h"
#include "DialogAbout.h"

QtStsPluginTestGui::QtStsPluginTestGui(QWidget* parent, Qt::WindowFlags flags)
	: QMainWindow(parent, flags)
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
		connection = QObject::connect(m_plugin, SIGNAL(stsConnected(bool)), this, SLOT(on_stsConnected(bool)));
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
		connection = QObject::connect(m_plugin, SIGNAL(trainDetailsReceived(QtSts::Train, QtSts::TrainEvent)), this, SLOT(on_trainDetailsReceived(QtSts::Train, QtSts::TrainEvent)));
		Q_ASSERT(connection);
		connection = QObject::connect(m_plugin, SIGNAL(timetableReceived(QtSts::Timetable)), this, SLOT(on_timetableReceived(QtSts::Timetable)));
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
	Q_ASSERT(m_plugin != nullptr);

	m_plugin->requestHeat();
}

void QtStsPluginTestGui::on_actionStitz_triggered()
{
	Q_ASSERT(m_plugin != nullptr);

	m_plugin->requestStitz();
}

void QtStsPluginTestGui::on_actionTrainList_triggered()
{
	Q_ASSERT(m_plugin != nullptr);

	m_plugin->requestTrainList();
}

void QtStsPluginTestGui::on_actionTrainInfo_triggered()
{
	Q_ASSERT(m_plugin != nullptr);

	bool ok = false;
	const int trainId = QInputDialog::getInt(this, tr("Request Train Info"), tr("Train ID"), 0, -2147483647, 2147483647, 1, &ok);
	if (ok)
	{
		m_plugin->requestTrainInfo(trainId);
	}
}

void QtStsPluginTestGui::on_actionTimetable_triggered()
{
	Q_ASSERT(m_plugin != nullptr);

	bool ok = false;
	const int trainId = QInputDialog::getInt(this, tr("Request Train Timetable"), tr("Train ID"), 0, -2147483647, 2147483647, 1, &ok);
	if (ok)
	{
		m_plugin->requestTimeTable(trainId);
	}
}

void QtStsPluginTestGui::on_actionRegisterEvent_triggered()
{
	Q_ASSERT(m_plugin != nullptr);

	DialogEvent dialog;

	const int result = dialog.exec();
	if (result == QDialog::Accepted)
	{
		m_plugin->registerEvent(dialog.trainId(), dialog.trainEvent());
	}
}

void QtStsPluginTestGui::on_actionAbout_triggered()
{
	DialogAbout dialog;
	dialog.exec();
}

void QtStsPluginTestGui::on_actionAboutQt_triggered()
{
	qApp->aboutQt();
}

void QtStsPluginTestGui::communicationFromSts(const QByteArray& dataFromSts)
{
	addToCommunicationLog(dataFromSts, false, true);
}

void QtStsPluginTestGui::communicationToSts(const QByteArray& dataToSts)
{
	addToCommunicationLog(dataToSts, true, true);
}

void QtStsPluginTestGui::on_stsConnected(bool connected)
{
	ui->logText->append(tr("signalConnected: connected=").append(connected ? QStringLiteral("TRUE") : QStringLiteral("FALSE")));
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

void QtStsPluginTestGui::on_trainDetailsReceived(QtSts::Train train, QtSts::TrainEvent event)
{
	QString sEvent(QStringLiteral("RESPONSE"));

	switch (event)
	{
	case QtSts::TrainEvent::ARRIVING:
		sEvent = QStringLiteral("ARRIVING");
		break;
	case QtSts::TrainEvent::DEPARTING:
		sEvent = QStringLiteral("DEPARTING");
		break;
	case QtSts::TrainEvent::INBOUND:
		sEvent = QStringLiteral("INBOUND");
		break;
	case QtSts::TrainEvent::OUTBOUND:
		sEvent = QStringLiteral("OUTBOUND");
		break;
	case QtSts::TrainEvent::NONE:
		sEvent = QStringLiteral("NONE");
		break;
	default:
		break;
	}

	QString log(tr("trainDetailsReceived: event=%1, train=").arg(sEvent));

	auto toString = [](bool b) ->QString {
		return b ? QStringLiteral("TRUE") : QStringLiteral("FALSE");
	};

	log.append(QStringLiteral("\n\ttrainId=%1").arg(train.id));
	log.append(QStringLiteral("\n\tname=\"%1\"").arg(train.name));
	log.append(QStringLiteral("\n\tfrom=\"%1\"").arg(train.from));
	log.append(QStringLiteral("\n\tto=\"%1\"").arg(train.to));
	log.append(QStringLiteral("\n\ttrack=\"%1\"").arg(train.track));
	log.append(QStringLiteral("\n\tplannedTrack=\"%1\"").arg(train.plannedTrack));
	log.append(QStringLiteral("\n\tdelay=%1").arg(train.delay));
	log.append(QStringLiteral("\n\tonTrack=%1").arg(toString(train.onTrack)));
	log.append(QStringLiteral("\n\tvisible=%1").arg(toString(train.visible)));
	
	ui->logText->append(log);
}

void QtStsPluginTestGui::on_timetableReceived(QtSts::Timetable timetable)
{
	QString log(tr("timetableReceived: timetable="));

	log.append(QStringLiteral("\n\ttrainId=%1").arg(timetable.trainId));
	for (const auto& entry : timetable.entries)
	{
		log.append(QStringLiteral("\n\t-\ttrack=\"%1\"").arg(entry.track));
		log.append(QStringLiteral("\n\t\tplannedTrack=\"%1\"").arg(entry.plannedTrack));
		log.append(QStringLiteral("\n\t\tarriving=\"%1\"").arg(entry.arriving));
		log.append(QStringLiteral("\n\t\tdeparting=\"%1\"").arg(entry.departing));
		log.append(QStringLiteral("\n\t\tflags=\"%1\"").arg(entry.flags));
	}

	ui->logText->append(log);
}

void QtStsPluginTestGui::addToCommunicationLog(const QByteArray& logData, bool outgoing, bool scrollToEnd)
{
	QString sHtml(QStringLiteral("<p style=\"color:"));
	sHtml.append(outgoing ? QStringLiteral("blue") : QStringLiteral("red"));
	sHtml.append(QStringLiteral("\">")).append(QString(logData).toHtmlEscaped());
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
