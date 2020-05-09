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
#include "DialogPlugin.h"
#include "ui_DialogPlugin.h"

DialogPlugin::DialogPlugin(QWidget* parent, Qt::WindowFlags flags)
	: QDialog(parent, flags)
	, ui(new Ui::DialogPlugin)
{
	ui->setupUi(this);
}

DialogPlugin::~DialogPlugin() = default;

void DialogPlugin::setName(const QString& name)
{
	ui->editName->setText(name);
}

QString DialogPlugin::name() const
{
	return ui->editName->text();
}

void DialogPlugin::setAuthor(const QString& author)
{
	ui->editAuthor->setText(author);
}

QString DialogPlugin::author() const
{
	return ui->editAuthor->text();
}

void DialogPlugin::setVersion(const QString& version)
{
	ui->editVersion->setText(version);
}

QString DialogPlugin::version() const
{
	return ui->editVersion->text();
}

void DialogPlugin::setDescription(const QString& description)
{
	ui->editDescription->setText(description);
}

QString DialogPlugin::description() const
{
	return ui->editDescription->text();
}
