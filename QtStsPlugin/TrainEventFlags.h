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
