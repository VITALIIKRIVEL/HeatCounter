#ifndef DIALOGLOGINDATABASE_H
#define DIALOGLOGINDATABASE_H

#include <QDialog>
#include <QMenuBar>
#include "formconnectionparams.h"

namespace Ui {
class DialogLoginDataBase;
}

class DialogLoginDataBase : public QDialog
{
    Q_OBJECT

public:
    explicit DialogLoginDataBase(QWidget *parent = nullptr);
    ~DialogLoginDataBase();

protected:
    void closeEvent(QCloseEvent *event);

signals:
    sendLoginPassword(QString, QString, bool);
    signalShowConnectParams();
    signalClose();

private slots:
    void on_toolButton_entrance_clicked();

    void on_checkBox_rememberLogin_clicked();

    void on_checkBox_rememberLogin_clicked(bool checked);

    void slotGetLogPasFromSettings(QString log, QString pas);

    void slotGetUserList(QStringList list);

    void actionIK_Settings();

private:
    Ui::DialogLoginDataBase *ui;

    FormConnectionParams * formConnectionParams;

    QMenuBar *menuBar;

    bool isRememberLogin;

    QString loadedUserFromSettings;

    QStringList userList;
};

#endif // DIALOGLOGINDATABASE_H
