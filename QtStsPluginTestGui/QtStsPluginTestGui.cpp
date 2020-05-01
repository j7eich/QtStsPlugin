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

void QtStsPluginTestGui::communicationFromSts(const QByteArray& data)
{
	addToCommunicationLog(data, false, true);
}

void QtStsPluginTestGui::communicationToSts(const QByteArray& data)
{
	addToCommunicationLog(data, true, true);
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
