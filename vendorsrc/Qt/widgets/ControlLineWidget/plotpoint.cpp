#include "plotpoint.h"

#define X 0
#define Y 1

PlotPoint::PlotPoint(QObject *parent) :
    QObject(parent), locked{false, false}
{

}
PlotPoint::PlotPoint(const QPointF &point, QObject *parent) :
    QPointF(point), QObject(parent), locked{false, false}
{
}

PlotPoint::PlotPoint(qreal x, qreal y, bool x_lock, bool y_lock, QObject *parent) :
    QPointF(x, y), QObject(parent), locked{x_lock, y_lock}
{
}


void PlotPoint::setX(qreal x)
{
    if (!locked[X]){
        QPointF::setX(x);
        emit(cordinates_changed(*this));
    }
}

void PlotPoint::setY(qreal y)
{
    if (!locked[Y]){
        QPointF::setY(y);
        emit(cordinates_changed(*this));
    }
}

PlotPoint &PlotPoint::set_coords(QPointF point)
{
    setX(point.x());
    setY(point.y());

    return *this;
}

PlotPoint &PlotPoint::operator +=(const QPointF &point)
{
        setX(this->x() + point.x());
        setY(this->y() + point.y());

        return *this;
}

PlotPoint &PlotPoint::operator -=(const QPointF &point)
{
        setX(this->x() - point.x());
        setY(this->y() - point.y());

        return *this;
}

PlotPoint &PlotPoint::operator *=(qreal factor)
{
    setX(this->x() * factor);
    setY(this->y() * factor);

    return *this;
}

PlotPoint &PlotPoint::operator /=(qreal divisor)
{
    setX(this->x() / divisor);
    setY(this->y() / divisor);

    return *this;
}

bool PlotPoint::x_lock() const { return locked[X]; }
bool PlotPoint::y_lock() const { return locked[Y]; }

void PlotPoint::set_x_lock(bool value) { locked[X] = value; }
void PlotPoint::set_y_lock(bool value) { locked[Y] = value; }
