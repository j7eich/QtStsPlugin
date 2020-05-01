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

#include <QDialog>
#include <QString>
#include <memory>

namespace Ui {
	class DialogPlugin;
}

class DialogPlugin : public QDialog
{
	Q_OBJECT
	Q_PROPERTY(QString name READ name WRITE setName)
	Q_PROPERTY(QString author READ author WRITE setAuthor)
	Q_PROPERTY(QString version READ version WRITE setVersion)
	Q_PROPERTY(QString description READ description WRITE setDescription)

public:
	DialogPlugin(QWidget* parent = nullptr, Qt::WindowFlags flags = 0);
	~DialogPlugin() override;

	QString name() const;
	QString author() const;
	QString version() const;
	QString description() const;

public Q_SLOTS:
	void setName(const QString& name);
	void setAuthor(const QString& author);
	void setVersion(const QString& version);
	void setDescription(const QString& description);

private:
	std::unique_ptr<Ui::DialogPlugin> ui;
};