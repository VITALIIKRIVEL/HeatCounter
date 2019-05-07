#ifndef DIALOGISRUNWITHOUTCONNECT_H
#define DIALOGISRUNWITHOUTCONNECT_H

#include <QDialog>

namespace Ui {
class DialogIsRunWithoutConnect;
}

class DialogIsRunWithoutConnect : public QDialog
{
    Q_OBJECT

public:
    void setText(QString str);

    explicit DialogIsRunWithoutConnect(QWidget *parent = nullptr);
    ~DialogIsRunWithoutConnect();

private:
    Ui::DialogIsRunWithoutConnect *ui;
};

#endif // DIALOGISRUNWITHOUTCONNECT_H
