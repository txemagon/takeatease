#include <QtGui>

#include "math.h"
#include "controllinewidget.h"
#include "plotdata.h"


#define RENDER_WIDTH0  800.
#define RENDER_HEIGHT0 500.
#define RENDER_XMARGIN 80
#define RENDER_YMARGIN RENDER_XMARGIN
#define TOTAL_RENDER_WIDTH  (RENDER_WIDTH0  + 2 * RENDER_XMARGIN)
#define TOTAL_RENDER_HEIGHT (RENDER_HEIGHT0 + 2 * RENDER_YMARGIN)

ControlLineWidget::ControlLineWidget(QWidget *parent)
    : QWidget(parent)
{

    knob_radius  =  7;

    dragging     = false;

    control_points.clear();
    control_points <<
                      QPointF(16., .7) <<
                      QPointF(3000., 1.) <<
                      QPointF(25000., 1.20);

    render_area = GraphicRenderer (this,
                                PlotData(control_points),
                                VisualizationData( RENDER_XMARGIN / 2,
                                                   RENDER_YMARGIN / 2,
                                                   TOTAL_RENDER_WIDTH  - RENDER_XMARGIN,
                                                   TOTAL_RENDER_HEIGHT - RENDER_YMARGIN,
                                                   RENDER_XMARGIN / 2,
                                                   RENDER_YMARGIN / 2),
                                knob_radius, LOGARITHMIC_AXIS);

    setWindowTitle("Control Line");
    resize(TOTAL_RENDER_WIDTH, TOTAL_RENDER_HEIGHT);
}


void ControlLineWidget::safety_paint(QPainter &painter,
                                     GraphicRenderer &object,
                                     void (GraphicRenderer::*operation)(QPainter &painter) )
{
    painter.save();
    object.set_coords(painter);
    if (operation)
        (object.*operation)(painter);
    painter.restore();
}

void ControlLineWidget::setup_canvas(QPainter &painter)
{
    painter.setRenderHint(QPainter::Antialiasing);
    painter.scale(width() / TOTAL_RENDER_WIDTH, height() / TOTAL_RENDER_HEIGHT);

    // Black background rectangle
    QLinearGradient gradient(QPointF(0., 0.),
                             QPointF(TOTAL_RENDER_WIDTH, TOTAL_RENDER_HEIGHT));
    gradient.setColorAt(0, Qt::black);
    gradient.setColorAt(1, Qt::gray);
    painter.fillRect(0, 0, TOTAL_RENDER_WIDTH, TOTAL_RENDER_HEIGHT, gradient);

    safety_paint(painter, render_area, &GraphicRenderer::setup_canvas);
}

void ControlLineWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    setup_canvas(painter);
    safety_paint(painter, render_area);
}

void ControlLineWidget::mousePressEvent(QMouseEvent *event)
{

    const QPoint mouse_position = render_area.from_app_to_canvas(event->pos());
    if (render_area.hovers(event->pos()))
        render_area.decide_dragging( mouse_position );
}

void ControlLineWidget::mouseMoveEvent(QMouseEvent *event)
{
    update();

}

void ControlLineWidget::mouseReleaseEvent(QMouseEvent *event)
{

}

bool ControlLineWidget::is_dragging() { return dragging; }

ControlLineWidget::~ControlLineWidget() { ; }
