#include "DialogEvent.h"
#include "ui_DialogEvent.h"

DialogEvent::DialogEvent(QWidget *parent)
	: QDialog(parent)
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
