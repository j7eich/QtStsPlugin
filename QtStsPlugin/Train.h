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

#include "qtstsplugin_global.h"
#include <QString>
#include <QMetaType>

namespace QtSts {

	enum class TrainEvent
	{
		NONE      = 0b00000000,
		INBOUND   = 0b00000001,
		OUTBOUND  = 0b00000010,
		ARRIVING  = 0b00000100,
		DEPARTING = 0b00001000,
		RESPONSE  = 0b10000000
	};

	class QTSTSPLUGIN_EXPORT Train
	{
	public:
		int id = 0;
		int delay = 0;
		bool onTrack = false;
		bool visible = false;
		QString name;
		QString from;
		QString to;
		QString track;
		QString plannedTrack;
	};
}

Q_DECLARE_METATYPE(QtSts::Train)
