#include "visualizationdata.h"


VisualizationData::VisualizationData(): area(), margin()
{
}

VisualizationData::VisualizationData(QRectF rect, qreal x_margin, qreal y_margin, bool inverted_y_coordinate):
    area(rect), margin(x_margin, y_margin)
{
}

VisualizationData::VisualizationData(QRectF rect, QPointF margin, bool inverted_y_coordinate):
    area(rect), margin(margin)
{
}

VisualizationData::VisualizationData(qreal x0, qreal y0,
                                     qreal total_width, qreal total_height,
                                     qreal x_margin, qreal y_margin, bool inverted_y_coordinate):
    area(x0, y0, total_width, total_height),
    margin(x_margin, y_margin)
{
}

qreal VisualizationData::width()  { return active_area().width(); }
qreal VisualizationData::height() { return active_area().height(); }

void VisualizationData::set_width(qreal width) { area.setWidth(width); }
void VisualizationData::set_height(qreal height) { area.setHeight(height);}

qreal VisualizationData::x0() { return area.left(); }
qreal VisualizationData::y0() { return area.top(); }

qreal VisualizationData::x1() { return x0() + x_margin(); }
qreal VisualizationData::y1() { return x1() + y_margin(); }

qreal VisualizationData::total_width() { return area.width();  }
qreal VisualizationData::total_height(){ return area.height(); }

qreal VisualizationData::x_margin() { return margin.x(); }
qreal VisualizationData::y_margin() { return margin.y(); }


QRectF VisualizationData::get_area() { return area; }

QPointF VisualizationData::get_margin() { return margin; }
QRectF VisualizationData::active_area()
{
    QRectF a_area = area.adjusted(  margin.x(),   margin.y(), - margin.x(), - margin.y());
    return a_area;
}

