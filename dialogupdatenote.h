#ifndef DIALOGUPDATENOTE_H
#define DIALOGUPDATENOTE_H

#include <QDialog>

namespace Ui {
class DialogUpdateNote;
}

class DialogUpdateNote : public QDialog
{
    Q_OBJECT

public:
    explicit DialogUpdateNote(QWidget *parent = nullptr);
    ~DialogUpdateNote();

private slots:
    void slotGetNoteData(QString serial, QString jsonString, int workplace, QString dateTime, int usableInt);

private:
    Ui::DialogUpdateNote *ui;
};

#endif // DIALOGUPDATENOTE_H
