#ifndef GUI_UTILS_H
#define GUI_UTILS_H

#include <QObject>
#include <QWidget>
#include <QTableWidget>
#include <QLayout>

//=========================================================================================================
namespace nayk {
//=========================================================================================================

//=========================================================================================================
class Gui
{
public:
    static void clearTableWidget(QTableWidget *table);
    static void clearTableWidgetBody(QTableWidget *table);
    static void removeTableWidgetRow(QTableWidget *table, int row);
    static void resizeTableToContents(QTableWidget *table);
    static void swapTableRows(QTableWidget *table, int row1, int row2);
    static void msgError(const QString &txt);
    static void msgWarning(const QString &txt);
    static void msgInfo(const QString &txt);
    static bool msgConfirm(const QString &txt);
    static int msgDialog(const QString &txt);
    static void msgError(QWidget *parent, const QString &txt);
    static void msgWarning(QWidget *parent, const QString &txt);
    static void msgInfo(QWidget *parent, const QString &txt);
    static bool msgConfirm(QWidget *parent, const QString &txt);
    static int msgDialog(QWidget *parent, const QString &txt);
    static QWidget* getMainWindowWidget();
    static void clearWidgets(QLayout *layout);
    static void swapWidgets(QWidget *w1, QWidget *w2);
    static bool saveControlValue(QWidget *w, const QString &fileName, const QString &prefixKey = QString());
    static bool loadControlValue(QWidget *w, const QString &fileName, const QString &prefixKey = QString());
    //
    Gui() = delete; // < std=c++11, обеспечивает запрет на создание любого экземпляра Gui
};

//=========================================================================================================
} // namespace nayk

//=========================================================================================================
#endif // GUI_UTILS_H
