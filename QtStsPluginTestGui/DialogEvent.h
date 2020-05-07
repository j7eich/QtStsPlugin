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
#include <memory>
#include "../QtStsPlugin/Train.h"

namespace Ui {
	class DialogEvent;
}

class DialogEvent : public QDialog
{
	Q_OBJECT

public:
	DialogEvent(QWidget* parent = nullptr, Qt::WindowFlags flags = Qt::WindowFlags());
	~DialogEvent() override;

	int trainId() const;
	QtSts::TrainEvent trainEvent() const;

private:
	std::unique_ptr<Ui::DialogEvent> ui;
};
