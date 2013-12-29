#ifndef PLOTDATA_H
#define PLOTDATA_H

#include <QVector>
#include <QPointF>

class PlotData
{
public:
    PlotData();
    PlotData(QVector<QPointF> point);

private:
    QVector<QPointF> point;
};

#endif // PLOTDATA_H
