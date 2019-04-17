#ifndef GRAPH_H
#define GRAPH_H

#include <QtCore>
#include <QWidget>
#include <QPainter>
#include <QImage>
#include <QColor>
#include <QRgb>
//=========================================================================================================
namespace nayk {
//=========================================================================================================

//=========================================================================================================
class Graph
{
public:
    static bool drawImage(QPainter* painter, const QString &fileName, double x, double y, double width, double height);
    static bool drawImage(QPainter* painter, const QString &fileName, const QRectF &rectF);
    static bool drawImage(QPainter* painter, const QImage &image, double x, double y, double width, double height);
    static bool drawImage(QPainter* painter, const QImage &image, const QRectF &rectF);
    static bool drawRectangle(QPainter* painter, double x, double y, double width, double height,
                              const QColor &bgColor, const QColor &borderColor, double borderWidth,
                              double topRadius, double bottomRadius);
    static bool drawRectangle(QPainter* painter, const QRectF &rect,
                              const QColor &bgColor, const QColor &borderColor, double borderWidth,
                              double topRadius, double bottomRadius);
    static bool drawRectangle(QPainter* painter, double x, double y, double width, double height,
                              const QJsonArray &bgColor, const QJsonArray &borderColor, double borderWidth,
                              double topRadius, double bottomRadius);
    static bool drawRectangle(QPainter* painter, const QRectF &rect,
                              const QJsonArray &bgColor, const QJsonArray &borderColor, double borderWidth,
                              double topRadius, double bottomRadius);
    static bool drawCircle(QPainter* painter, double x, double y, double radius,
                           const QColor &bgColor, const QColor &borderColor, double borderWidth );
    static bool drawCircle(QPainter* painter, double x, double y, double radius,
                           const QJsonArray &bgColor, const QJsonArray &borderColor, double borderWidth );
    static bool drawCircle(QPainter* painter, const QPointF &centerPoint, double radius,
                           const QColor &bgColor, const QColor &borderColor, double borderWidth );
    static bool drawCircle(QPainter* painter, const QPointF &centerPoint, double radius,
                           const QJsonArray &bgColor, const QJsonArray &borderColor, double borderWidth );
    static bool drawCircle(QPainter* painter, const QPoint &centerPoint, double radius,
                           const QJsonArray &bgColor, const QJsonArray &borderColor, double borderWidth );
    static bool drawCircle(QPainter* painter, const QPoint &centerPoint, double radius,
                           const QColor &bgColor, const QColor &borderColor, double borderWidth );
    static bool drawText(QPainter* painter, double x, double y, const QString &text,
                         const QString &fontFace, double fontSize, double fontWeight, const QColor &color,
                         int align = Qt::AlignLeft | Qt::AlignTop, const QColor &shadowColor = QColor(0,0,0,0),
                         int shadow_cx = 0, int shadow_cy = 0);
    static bool drawText(QPainter* painter, double x, double y, const QString &text,
                         const QString &fontFace, double fontSize, double fontWeight, const QJsonArray &color,
                         int align = Qt::AlignLeft | Qt::AlignTop, const QJsonArray &shadowColor = QJsonArray(),
                         int shadow_cx = 0, int shadow_cy = 0);
    static bool drawText(QPainter* painter, QPointF pos, const QString &text,
                         const QString &fontFace, double fontSize, double fontWeight, const QColor &color,
                         int align = Qt::AlignLeft | Qt::AlignTop, const QColor &shadowColor = QColor(0,0,0,0),
                         int shadow_cx = 0, int shadow_cy = 0);
    static bool drawText(QPainter* painter, QPointF pos, const QString &text,
                         const QString &fontFace, double fontSize, double fontWeight, const QJsonArray &color,
                         int align = Qt::AlignLeft | Qt::AlignTop, const QJsonArray &shadowColor = QJsonArray(),
                         int shadow_cx = 0, int shadow_cy = 0);

    static QColor colorFromArray(const QJsonArray &arr);
    static QColor colorFromArray(const QByteArray &arr);
    static QColor colorFromArray(const QVector<quint8> &arr);

    //
    Graph() = delete; // < std=c++11, обеспечивает запрет на создание любого экземпляра Graph
};

//=========================================================================================================
} // namespace nayk

//=========================================================================================================
#endif // GRAPH_H
