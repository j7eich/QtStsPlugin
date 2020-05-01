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
#include <QApplication>

#include "../QtStsPlugin/Plugin.h"
#include "DialogPlugin.h"

QtStsPluginTestGui::QtStsPluginTestGui(QWidget *parent)
	: QMainWindow(parent)
	, ui()
	, m_plugin(nullptr)
{
	ui.setupUi(this);
	ui.actionDestroy->setEnabled(false);
}

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
		ui.actionInstantiate->setEnabled(false);
		m_plugin = new QtSts::Plugin(dialog.name(), dialog.author(), dialog.version(), dialog.description(), this);
		ui.actionDestroy->setEnabled(true);
	}
}

void QtStsPluginTestGui::on_actionDestroy_triggered()
{
	Q_ASSERT(m_plugin != nullptr);
	ui.actionInstantiate->setEnabled(true);
	delete m_plugin;
	m_plugin = nullptr;
	ui.actionDestroy->setEnabled(false);
}
