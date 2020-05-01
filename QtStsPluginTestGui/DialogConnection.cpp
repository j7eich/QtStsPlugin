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
#include "DialogConnection.h"
#include "ui_DialogConnection.h"

DialogConnection::DialogConnection(QWidget* parent, Qt::WindowFlags flags)
	: QDialog(parent, flags)
	, ui(new Ui::DialogConnection)
{
	ui->setupUi(this);
}

DialogConnection::~DialogConnection() = default;

QString DialogConnection::hostname() const
{
	return ui->hostnameEdit->text();
}

unsigned short DialogConnection::port() const
{
	return ui->tcpPortSpin->value();
}

QAbstractSocket::NetworkLayerProtocol DialogConnection::protocol() const
{
	QAbstractSocket::NetworkLayerProtocol protocol = QAbstractSocket::UnknownNetworkLayerProtocol;
	switch (ui->ipProtocolCombo->currentIndex())
	{
	default:
	case 0:
		protocol = QAbstractSocket::AnyIPProtocol;
		break;
	case 1:
		protocol = QAbstractSocket::IPv6Protocol;
		break;
	case 2:
		protocol = QAbstractSocket::IPv4Protocol;
		break;
	}
	return protocol;
}

void DialogConnection::setHostname(const QString& hostname)
{
	ui->hostnameEdit->setText(hostname);
}

void DialogConnection::setPort(unsigned short port)
{
	ui->tcpPortSpin->setValue(port);
}

void DialogConnection::setProtocol(QAbstractSocket::NetworkLayerProtocol protocol)
{
	switch (protocol)
	{
	case QAbstractSocket::IPv4Protocol:
		ui->ipProtocolCombo->setCurrentIndex(2);
		break;
	case QAbstractSocket::IPv6Protocol:
		ui->ipProtocolCombo->setCurrentIndex(1);
		break;
	case QAbstractSocket::AnyIPProtocol:
	default:
		ui->ipProtocolCombo->setCurrentIndex(0);
		break;
	}
}
