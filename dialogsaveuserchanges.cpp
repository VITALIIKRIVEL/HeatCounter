#include "dialogsaveuserchanges.h"
#include "ui_dialogsaveuserchanges.h"
#include <QPushButton>

DialogSaveUserChanges::DialogSaveUserChanges(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogSaveUserChanges)
{
    ui->setupUi(this);

    ui->buttonBox->button(QDialogButtonBox::Cancel)->setText(tr("Отмена"));
}

DialogSaveUserChanges::~DialogSaveUserChanges()
{
    delete ui;
}

void DialogSaveUserChanges::slotGetText(QString str)
{
    ui->label->setText(str);
}
