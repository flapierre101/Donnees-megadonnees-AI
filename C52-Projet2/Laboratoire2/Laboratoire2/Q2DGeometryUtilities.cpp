#include "Q2DGeometryUtilities.h"


#include <QVector2D>



qreal Q2DGeometryUtilities::perimeter(QRect const& rectangle)
{
    return 2.0 * (static_cast<double>(rectangle.width()) + static_cast<double>(rectangle.height()));
}

qreal Q2DGeometryUtilities::perimeter(QRectF const& rectangle)
{
    return 2.0 * (rectangle.width() + rectangle.height());
}

qreal Q2DGeometryUtilities::perimeter(QPolygon const& polygon)
{
    qreal perimeter{};
    size_t prevIndex{ polygon.size() - size_t{ 1 } };
    size_t curIndex{};
    while (curIndex < polygon.size()) {
        perimeter += QVector2D(polygon[curIndex] - polygon[prevIndex]).length();

        prevIndex = curIndex;
        ++curIndex;
    }

    return perimeter;
}

qreal Q2DGeometryUtilities::perimeter(QPolygonF const& polygon)
{
    qreal perimeter{};
    size_t prevIndex{ polygon.size() - size_t{ 1 } };
    size_t curIndex{};
    while (curIndex < polygon.size()) {
        perimeter += QVector2D(polygon[curIndex] - polygon[prevIndex]).length();

        prevIndex = curIndex;
        ++curIndex;
    }

    return perimeter;
}



qreal Q2DGeometryUtilities::area(QRect const& rectangle)
{
    return static_cast<double>(rectangle.width()) * static_cast<double>(rectangle.height());
}

qreal Q2DGeometryUtilities::area(QRectF const& rectangle)
{
    return rectangle.width() * rectangle.height();
}

qreal Q2DGeometryUtilities::area(QPolygon const& polygon)
{
    qreal area{};
    size_t prevIndex{ polygon.size() - size_t{ 1 } };
    size_t curIndex{};
    while (curIndex < polygon.size()) {
        area += (static_cast<double>(polygon[curIndex].x()) - polygon[prevIndex].x()) * (polygon[curIndex].y() + polygon[prevIndex].y()) / 2.0; // trapezoidal integration around shape

        prevIndex = curIndex;
        ++curIndex;
    }

    return qAbs(area);
}

qreal Q2DGeometryUtilities::area(QPolygonF const& polygon)
{
    qreal area{};
    size_t prevIndex{ polygon.size() - size_t{ 1 } };
    size_t curIndex{};
    while (curIndex < polygon.size()) {
        area += (polygon[curIndex].x() - polygon[prevIndex].x()) * (polygon[curIndex].y() + polygon[prevIndex].y()) / 2.0; // trapezoidal integration around shape

        prevIndex = curIndex;
        ++curIndex;
    }

    return qAbs(area);
}
