#ifndef FORMLOG_H
#define FORMLOG_H

#include <QWidget>

namespace Ui {
class FormLog;
}

class FormLog : public QWidget
{
    Q_OBJECT

public:
    explicit FormLog(QWidget *parent = nullptr);
    ~FormLog();

private slots:
    void slotGetLog(QString);

    void on_toolButton_clear_clicked();

private:
    Ui::FormLog *ui;
};

#endif // FORMLOG_H
