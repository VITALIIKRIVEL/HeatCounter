#ifndef FORMUSERCONTROL_H
#define FORMUSERCONTROL_H

#include <QWidget>
#include "dialogsaveuserchanges.h"

namespace Ui {
class Formusercontrol;
}

class Formusercontrol : public QWidget
{
    Q_OBJECT

public:
    explicit Formusercontrol(QWidget *parent = nullptr);
    ~Formusercontrol();

signals:
    sendVectorToQueryAdd(QVector<QString> vector);
    sendVectorToQueryChange(QVector<QString> vector);
    sendVectorToQueryRemove(QVector<QString> vector);

    sendTextToDialog(QString str);

private slots:
    void slotGetListUserTable(QStringList list);

    void on_tableWidget_userTable_cellClicked(int row, int column);

    void on_lineEdit_humanName_textChanged(const QString &arg1);

    void on_lineEdit_password_textChanged(const QString &arg1);

    void on_lineEdit_passwordRepeat_textChanged(const QString &arg1);

    void on_comboBox_noteType_currentIndexChanged(const QString &arg1);

    void on_checkBox_isActive_clicked(bool checked);

    void on_toolButton_add_clicked();

    void on_toolButton_change_clicked();

    void on_toolButton_delete_clicked();

    void slotGetCurrentUser(QString user);

private:
    Ui::Formusercontrol *ui;

    DialogSaveUserChanges * dialogSaveUserChanges;

    QStringList allUserTable;

    QString curUser;
    QString curUserFromLine;

    QString curPassword;
    QString curPasswordFromLine;
    QString curPasswordFromLineRep;

    QString curTypeOfNote;
    QString curTypeOfNoteFromCombo;

    QString curStatus;
    QString curStatusFromCheckBox;

    QVector<QString> vectorToQuery;


};

#endif // FORMUSERCONTROL_H
