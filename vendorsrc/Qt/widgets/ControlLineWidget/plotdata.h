#ifndef PLOTDATA_H
#define PLOTDATA_H

#include <QtCore>
#include <QVector>
#include <QPointF>

class PlotData : public QObject
{
   Q_OBJECT
public:
    PlotData();
    PlotData(QVector<QPointF> point);

    QVector<QPointF> &points();
    void set_coords(int point, QPointF coords);

signals:
    void coordinates_changed(int point_index, QPointF point);

private:
    QVector<QPointF> point;
};

#endif // PLOTDATA_H
