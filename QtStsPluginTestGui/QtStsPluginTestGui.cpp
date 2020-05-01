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

void QtStsPluginTestGui::communicationFromSts(const QByteArray& data)
{
	QString sIn = QString(data).toHtmlEscaped();
	sIn = QStringLiteral("<p style=\"color:red;\">%1</p><br />").arg(sIn);
	ui->communicationText->insertHtml(sIn);
}

void QtStsPluginTestGui::communicationToSts(const QByteArray& data)
{
	QString sOut = QString(data).toHtmlEscaped();
	sOut = QStringLiteral("<p style=\"color:blue;\">%1</p><br />").arg(sOut);
	ui->communicationText->insertHtml(sOut);
}
