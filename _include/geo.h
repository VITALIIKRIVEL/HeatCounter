#ifndef GEO_H
#define GEO_H

#include <QtCore>

//=========================================================================================================
namespace nayk {
//=========================================================================================================
const double earth_a            = 6378137.0;           // Большая полуось эллипсоида (Экваториальный радиус)
const double earth_b            = 6356752.3142;        // Малая полуось эллипсоида
const double earth_e            = 0.0818191908426;     // Эксцентриситет
const double earth_equator      = 40075016.685578488;  // Длина экватора в метрах
const double earth_half_equator = earth_equator / 2.0; // половина экватора
//=========================================================================================================
class Geo
{
public:
    static QPointF coordGeoToMap(double latitude, double longitude, double map_width, double cx=0, double cy=0);
    static QPointF coordMapToGeo(double x, double y, double map_width, double cx=0, double cy=0);
    //
    Geo() = delete; // < std=c++11, обеспечивает запрет на создание любого экземпляра Geo
};

//=========================================================================================================
} // namespace nayk

//=========================================================================================================
#endif // GEO_H
