#ifndef PLOTDATA_H
#define PLOTDATA_H

#include <QtCore>
#include <QVector>
#include <QPointF>
#include "plotpoint.h"

class PlotData : public QObject
{
   Q_OBJECT
public:
    PlotData();
    PlotData(QVector<QPointF> point);

    QVector<PlotPoint *> &points();
    void set_coords(int point, QPointF coords);

signals:
    void coordinates_changed(int point_index, const PlotPoint &point);

private:
    QVector<PlotPoint *> point;
};

#endif // PLOTDATA_H
