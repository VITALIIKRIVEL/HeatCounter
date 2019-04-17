#ifndef _XLS_H
#define _XLS_H

#include <QtCore>
#include <QObject>
#include <ActiveQt/QAxObject>
#include <ActiveQt/QAxBase>

//=========================================================================================================
namespace nayk {
//=========================================================================================================
typedef enum {
    xlVAlignBottom = -4107,
    xlVAlignCenter = -4108,
    xlVAlignDistributed = -4117,
    xlVAlignJustify = -4130,
    xlVAlignTop = -4160
} XlVAlign;

typedef enum {
    xlHAlignCenter = -4108,
    xlHAlignCenterAcrossSelection = 7,
    xlHAlignDistributed = -4117,
    xlHAlignFill = 5,
    xlHAlignGeneral = 1,
    xlHAlignJustify = -4130,
    xlHAlignLeft = -4131,
    xlHAlignRight = -4152
} XlHAlign;

typedef enum {
    xlDownward = -4170,
    xlHorizontal = -4128,
    xlUpward = -4171,
    xlVertical = -4166
} XlOrientation;

typedef enum {
    xlHairline = 1,
    xlMedium = -4138,
    xlThick = 4,
    xlThin = 2
} XlBorderWeight;

typedef enum {
    xlContinuous = 1,
    xlDash = -4115,
    xlDashDot = 4,
    xlDashDotDot = 5,
    xlDot = -4118,
    xlDouble = -4119,
    xlSlantDashDot = 13,
    xlLineStyleNone = -4142
} XlLineStyle;
//=========================================================================================================
class Xls: QObject
{
	Q_OBJECT

public:
    Xls(QObject *parent = 0, const QString &fileName = QString(), bool visible = false);
    ~Xls();
    bool isValid() { return _ok; }
    QString lastError() { return _lastError; }
    QString fileName() { return _fileName; }
    bool setVisible(bool visible = true);
    bool save(const QString &fileName = QString());
    bool close();
    bool addSheet(const QString &templateFile = QString());
    bool delSheet(int num);
    int sheetsCount();
    bool setSheetName(int num, const QString &name);
    bool addRow(int sheetNum, int rowNum, int rowCount = 1);
    bool setCell(int sheetNum, int rowNum, int colNum, int cellCount = 1, bool mergeCells = false,
                 XlBorderWeight borderWeight = xlThin, XlLineStyle lineStyle = xlContinuous, const QString &text = QString());
    static QString cellName(quint32 row, quint8 col);

private:
    QAxObject* excel {nullptr};
    QAxObject* workbooks {nullptr};
    QAxObject* workbook {nullptr};
    QAxObject* sheets {nullptr};
    bool _ok {false};
    QString _fileName {""};
    QString _lastError {""};
    bool _visible {false};
    //
    QAxObject* getSheet(int num);
};

//=========================================================================================================
} // namespace nayk

//=========================================================================================================
#endif // _XLS_H
