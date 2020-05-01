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

#include <QFlags>

namespace QtSts {

	enum TrainEvent
	{
		NONE      = 0b0000,
		INBOUND   = 0b0001,
		ARRIVAL   = 0b0010,
		DEPARTURE = 0b0100,
		OUTBOUND  = 0b1000
	};
	Q_DECLARE_FLAGS(TrainEvents, TrainEvent)

}

Q_DECLARE_OPERATORS_FOR_FLAGS(QtSts::TrainEvents)
