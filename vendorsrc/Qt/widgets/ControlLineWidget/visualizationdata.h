#ifndef VISUALIZATIONDATA_H
#define VISUALIZATIONDATA_H

#include <QPointF>
#include <QRectF>

class VisualizationData
{
public:
    VisualizationData();
    VisualizationData(QRectF rect, QPointF margin);
    VisualizationData(QRectF rect, qreal x_margin, qreal y_margin);
    VisualizationData(qreal x0, qreal y0,
                      qreal total_width, qreal total_height,
                      qreal x_margin, qreal y_margin);


    qreal width();
    qreal height();
    qreal x0();  // frame coordinate
    qreal y0();
    qreal x1();  // content coordinate
    qreal y1();
    qreal total_width();
    qreal total_height();
    qreal x_margin();
    qreal y_margin();
    QRectF get_area();
    QRectF active_area();

private:
    QRectF area;
    QPointF margin;

};

#endif // VISUALIZATIONDATA_H
