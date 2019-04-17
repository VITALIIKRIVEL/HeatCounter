#ifndef DIALOGWRITINGDB_H
#define DIALOGWRITINGDB_H

#include <QDialog>

namespace Ui {
class DialogWritingDB;
}

class DialogWritingDB : public QDialog
{
    Q_OBJECT

public:
    explicit DialogWritingDB(QWidget *parent = nullptr);
    ~DialogWritingDB();

private:
    Ui::DialogWritingDB *ui;
};

#endif // DIALOGWRITINGDB_H
