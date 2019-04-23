#ifndef DIALOGSAVEUSERCHANGES_H
#define DIALOGSAVEUSERCHANGES_H

#include <QDialog>

namespace Ui {
class DialogSaveUserChanges;
}

class DialogSaveUserChanges : public QDialog
{
    Q_OBJECT

public:
    explicit DialogSaveUserChanges(QWidget *parent = nullptr);
    ~DialogSaveUserChanges();

private slots:
    void slotGetText(QString str);

private:
    Ui::DialogSaveUserChanges *ui;
};

#endif // DIALOGSAVEUSERCHANGES_H
