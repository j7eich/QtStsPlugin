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
#include "DialogEvent.h"
#include "ui_DialogEvent.h"

DialogEvent::DialogEvent(QWidget* parent, Qt::WindowFlags flags)
	: QDialog(parent, flags)
	, ui(new Ui::DialogEvent)
{
	ui->setupUi(this);
}

DialogEvent::~DialogEvent() = default;

int DialogEvent::trainId() const
{
	return ui->spinTrainId->value();
}

QtSts::TrainEvent DialogEvent::trainEvent() const
{
	QtSts::TrainEvent e = QtSts::TrainEvent::NONE;

	switch (ui->comboEvent->currentIndex())
	{
	case 0:
		e = QtSts::TrainEvent::INBOUND;
		break;
	case 1:
		e = QtSts::TrainEvent::OUTBOUND;
		break;
	case 2:
		e = QtSts::TrainEvent::ARRIVING;
		break;
	case 3:
		e = QtSts::TrainEvent::DEPARTING;
		break;
	default:
		break;
	}

	return e;
}
