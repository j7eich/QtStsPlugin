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
	DialogEvent(QWidget* parent = nullptr);
	~DialogEvent() override;

	int trainId() const;
	QtSts::TrainEvent trainEvent() const;

private:
	std::unique_ptr<Ui::DialogEvent> ui;
};
