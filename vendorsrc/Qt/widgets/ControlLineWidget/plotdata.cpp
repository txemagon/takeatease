#include "plotdata.h"

PlotData::PlotData()
{
}

PlotData::PlotData(QVector<QPointF> point)
{
    this->point = point;
}

QVector<QPointF> &PlotData::points()
{
    return point;
}
