#include "dialogupdatenote.h"
#include "ui_dialogupdatenote.h"
#include <QPushButton>

DialogUpdateNote::DialogUpdateNote(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogUpdateNote)
{
    ui->setupUi(this);

    ui->buttonBox->button(QDialogButtonBox::Cancel)->setText(tr("Отмена"));

}

DialogUpdateNote::~DialogUpdateNote()
{
    delete ui;
}

void DialogUpdateNote::slotGetNoteData(QString pcb, QString jsonString, int workplace, QString dateTime, int usableInt)
{

    QString jsonStringTmp = jsonString.remove('{');
    jsonStringTmp = jsonString.remove('}');

    QStringList jsonList = jsonStringTmp.split(',');

    int listSize = jsonList.size();

    if(jsonList.size()<4) {
        for(int m=0; m<(4 - listSize); m++) {
            jsonList.append(QString());
        }
    }

    dateTime = dateTime.replace('T', ' T');
    dateTime.remove(".000");

    QString res = jsonList.at(2);
    res.remove('"');
    res.remove("Usable");
    res.remove(":");

    QString eng = jsonList.at(3);
    eng.remove('"');
    eng.remove("Engeneer");
    eng.remove(":");

    QString usableStr;

    if(usableInt == 1) {
        usableStr = tr("Годен");
    }
    else {
        usableStr = tr("Не годен");
    }


    ui->label->setText( tr("Запись с таким номером PCB уже существует. Рабочее место: ") +
                       QString::number(workplace) + "\n" + "\n" +
                       pcb + ";  " +
                       tr("Время записи: ") + dateTime + ";  " +
                       tr("Результат:") + usableStr + ";  " +
                    /*   tr("Инженер:") + eng +       */
                /*       jsonString + "\n" + "\n" +       */
                       "\n" + "\n" +
                       tr("Перезаписать результат проверки?") );
}
