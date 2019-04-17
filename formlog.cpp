#include "formlog.h"
#include "ui_formlog.h"

FormLog::FormLog(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormLog)
{
    ui->setupUi(this);
}

FormLog::~FormLog()
{
    delete ui;
}

void FormLog::slotGetLog(QString str)
{
    ui->textBrowser_log->append(str);
}

void FormLog::on_toolButton_clear_clicked()
{
    ui->textBrowser_log->clear();
}
