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
#include <QAbstractSocket>
#include <memory>

namespace Ui {
	class DialogConnection;
}

class DialogConnection : public QDialog
{
	Q_OBJECT
	Q_PROPERTY(QString hostname READ hostname WRITE setHostname)
	Q_PROPERTY(unsigned short port READ port WRITE setPort)
	Q_PROPERTY(QAbstractSocket::NetworkLayerProtocol protocol READ protocol WRITE setProtocol)

public:
	DialogConnection(QWidget* parent = nullptr, Qt::WindowFlags flags = Qt::WindowFlags());
	~DialogConnection() override;

	QString hostname() const;
	unsigned short port() const;
	QAbstractSocket::NetworkLayerProtocol protocol() const;

public Q_SLOTS:
	void setHostname(const QString& hostname);
	void setPort(unsigned short port);
	void setProtocol(QAbstractSocket::NetworkLayerProtocol protocol);

private:
	std::unique_ptr<Ui::DialogConnection> ui;
};
