#include "plotdata.h"

PlotData::PlotData()
{
    point.clear();
}

PlotData::PlotData(QVector<QPointF> point)
{
    this->point = point;
}
