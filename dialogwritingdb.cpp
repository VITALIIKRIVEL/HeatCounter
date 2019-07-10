#include "dialogwritingdb.h"
#include "ui_dialogwritingdb.h"
#include <QPushButton>

DialogWritingDB::DialogWritingDB(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogWritingDB)
{
    ui->setupUi(this);

    ui->buttonBox->button(QDialogButtonBox::Cancel)->setText(tr("Отмена"));
}

DialogWritingDB::~DialogWritingDB()
{
    delete ui;
}

void DialogWritingDB::setText(QString text)
{
    ui->label_text->setText(text);
}
