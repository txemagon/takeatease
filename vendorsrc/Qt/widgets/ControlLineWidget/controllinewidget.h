#ifndef CONTROLLINEWIDGET_H
#define CONTROLLINEWIDGET_H

#include <QtGui/QWidget>

#include "visualizationdata.h"
#include "graphicrenderer.h"


#define MID_POINT 1
#define SUB_DIV   2
#define MAIN_DIV  4
#define ALL_DIV   (MAIN_DIV | SUB_DIV)


class GraphicRenderer;

class ControlLineWidget : public QWidget
{
    Q_OBJECT
    
public:
    ControlLineWidget(QWidget *parent = 0);
    ~ControlLineWidget();

    void mousePressEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);
    bool is_dragging(void);
    qreal initial_width();
    qreal initial_height();

protected:
    void paintEvent(QPaintEvent *event);

    void setup_canvas(QPainter &painter);
    void safety_paint(QPainter &painter,
                      GraphicRenderer &object,
                      void (GraphicRenderer::*operation)(QPainter &painter) = &GraphicRenderer::paint);
        QPoint from_app_to_canvas(GraphicRenderer panel, const QPoint &pressed_point);

private:
    qreal knob_radius;
    QVector<QPointF> control_points;

    bool dragging;
    QPoint mouse_pressed_position;

    GraphicRenderer render_area;

};

#endif // CONTROLLINEWIDGET_H
