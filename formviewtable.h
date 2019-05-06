#ifndef FORMVIEWTABLE_H
#define FORMVIEWTABLE_H

#include <QWidget>
#include <QDate>
#include <QMap>

namespace Ui {
class FormViewTable;
}

class FormViewTable : public QWidget
{
    Q_OBJECT

public:
    explicit FormViewTable(QWidget *parent = nullptr);
    ~FormViewTable();

signals:
    updateTable();

private slots:
    void slotGetTableList(QStringList tableList, QString userName, QMap<int, QString> userMap, QStringList userList, QList<int> intList);

    void on_toolButton_updateTable_clicked();

    void on_dateEdit_start_dateChanged(const QDate &date);

    void on_dateEdit_stop_dateChanged(const QDate &date);

    void on_checkBox_allNotes_clicked();

    void on_checkBox_allNotes_clicked(bool checked);

    void on_comboBox_user_currentIndexChanged(const QString &arg1);

    void on_comboBox_type_currentIndexChanged(const QString &arg1);

    void on_comboBox_usable_currentIndexChanged(const QString &arg1);

    void slotGetUserList(QStringList userTable);

    void on_toolButton_filtersReset_clicked();

    void on_checkBox_date_clicked(bool checked);

private:
    Ui::FormViewTable *ui;

    QDate dateStart;
    QDate dateStop;
    bool isAllNotesDisplay;
    bool isDateUse;

    int userId;

    QMap<QString, int> mapWorkUser;

    QString typeCounterStr;

    QString userStrGlobal;

    QString usableStrGlobal;
};

#endif // FORMVIEWTABLE_H
