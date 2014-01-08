#include "plotdata.h"

PlotData::PlotData() {
    this->point.clear();
}

PlotData::PlotData(QVector<QPointF> point) { this->point = point; }
QVector<QPointF> &PlotData::points(){ return point; }

void PlotData::set_coords(int point, QPointF coords)
{
    this->point[point] = coords;
    emit(coordinates_changed(point, this->point[point]));
}
