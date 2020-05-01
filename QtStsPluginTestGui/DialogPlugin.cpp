#include "DialogPlugin.h"
#include "ui_DialogPlugin.h"

DialogPlugin::DialogPlugin(QWidget* parent, Qt::WindowFlags flags)
	: QDialog(parent, flags)
	, ui(new Ui::DialogPlugin())
{
	ui->setupUi(this);
}

DialogPlugin::~DialogPlugin() = default;

void DialogPlugin::setName(const QString& name)
{
	ui->editName->setText(name);
}

QString DialogPlugin::name() const
{
	return ui->editName->text();
}

void DialogPlugin::setAuthor(const QString& author)
{
	ui->editAuthor->setText(author);
}

QString DialogPlugin::author() const
{
	return ui->editAuthor->text();
}

void DialogPlugin::setVersion(const QString& version)
{
	ui->editVersion->setText(version);
}

QString DialogPlugin::version() const
{
	return ui->editVersion->text();
}

void DialogPlugin::setDescription(const QString& description)
{
	ui->editDescription->setText(description);
}

QString DialogPlugin::description() const
{
	return ui->editDescription->text();
}
