#include "plotdata.h"

PlotData::PlotData() {
    this->point.clear();
}

PlotData::PlotData(QVector<QPointF> point) {

    QVector<QPointF>::iterator i = point.begin();

    while(i != point.end()) {
        this->point << new PlotPoint(*i);
    }
}
QVector<PlotPoint *> &PlotData::points(){ return point; }

void PlotData::set_coords(int point, QPointF coords)
{
    this->point[point]->set_coords(coords);
    emit(coordinates_changed(point, *(this->point[point])) );
}

void PlotData::remove(int point_number)
{
    if (point.count() > 2)
        point.remove(point_number);
}
