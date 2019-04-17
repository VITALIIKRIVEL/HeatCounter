#include <QtMath>
#include "geo.h"

namespace nayk {
//=======================================================================================================
QPointF Geo::coordGeoToMap(double latitude, double longitude, double map_width, double cx, double cy)
{
    double aa = map_width / earth_equator; // кол-во пикселей в метре
    double lat = qDegreesToRadians( latitude );
    double lon = qDegreesToRadians( longitude );
    double x = earth_a * lon;
    double y = earth_a * qLn( qTan( M_PI_4 + lat/2.0) *
                        qPow((1 - earth_e * qSin(lat))/(1 + earth_e * qSin(lat)), earth_e/2.0) );
    return QPointF( cx + (x + earth_half_equator) * aa, cy + map_width - (y + earth_half_equator) * aa );
}
//=======================================================================================================
QPointF Geo::coordMapToGeo(double x, double y, double map_width, double cx, double cy)
{
    double aa = map_width / earth_equator; // кол-во пикселей в метре
    x = (x - cx)/aa - earth_half_equator;
    y = (map_width + cy - y)/aa - earth_half_equator;
    double ts = qExp( -y / earth_a );
    double phi = M_PI_2 - 2.0 * qAtan(ts);
    int i = 0;
    double dphi = 1.0;
    while ((qAbs(dphi) > 0.000000001) && (i++ < 15)) {
        double con = earth_e * qSin(phi);
        dphi = M_PI_2 - 2.0 * qAtan(ts * qPow((1.0 - con) / (1.0 + con), earth_e)) - phi;
        phi += dphi;
     }
     return QPointF( qRadiansToDegrees( x / earth_a ),  qRadiansToDegrees(phi)  );
}
//=======================================================================================================
} // namespace nayk

