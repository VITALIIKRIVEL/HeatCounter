#ifndef FORMCONNECTIONPARAMS_H
#define FORMCONNECTIONPARAMS_H

#include <QWidget>

namespace Ui {
class FormConnectionParams;
}

class FormConnectionParams : public QWidget
{
    Q_OBJECT

public:
    explicit FormConnectionParams(QWidget *parent = nullptr);
    ~FormConnectionParams();

signals:
    sendParamsConnection(QStringList paramsList);

private slots:
    void on_toolButton_OK_clicked();
    void slotGetConnectParamsFromMW(QStringList list);

private:
    Ui::FormConnectionParams *ui;
};

#endif // FORMCONNECTIONPARAMS_H
