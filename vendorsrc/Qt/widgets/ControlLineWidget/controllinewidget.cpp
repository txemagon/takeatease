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

#define INITIAL_WIDTH  TOTAL_RENDER_WIDTH
#define INITIAL_HEIGHT TOTAL_RENDER_HEIGHT

ControlLineWidget::ControlLineWidget(QWidget *parent)
    : QWidget(parent)
{

    knob_radius  =  7;

    mouse_pressed_position = QPoint(-1, -1);
    dragging     = false;

    control_points.clear();
    control_points <<
                      QPointF(16., .7) <<
                      QPointF(3000., 1.) <<
                      QPointF(25000., 1.20);

    PlotData data(control_points);

    render_area = GraphicRenderer (this,
                                data,
                                VisualizationData( RENDER_XMARGIN / 2,
                                                   RENDER_YMARGIN / 2,
                                                   TOTAL_RENDER_WIDTH  - RENDER_XMARGIN,
                                                   TOTAL_RENDER_HEIGHT - RENDER_YMARGIN,
                                                   RENDER_XMARGIN / 2,
                                                   RENDER_YMARGIN / 2, true),
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

QPoint ControlLineWidget::from_app_to_canvas(GraphicRenderer panel, const QPoint &pressed_point)
{
    VisualizationData visual = panel.get_visualization_data();
    QPoint top = visual.get_area().topLeft().toPoint();
    QPointF absolute_point = pressed_point;
    absolute_point.setX(absolute_point.x() * initial_width()  / width());
    absolute_point.setY(absolute_point.y() * initial_height() / height());
    QPoint corner = absolute_point.toPoint() - top;

    if (visual.inverted_y_coordinate)
        corner.setY(visual.total_height() - corner.y());

    return corner;
}

void ControlLineWidget::mousePressEvent(QMouseEvent *event)
{

    mouse_pressed_position = event->pos();
    QPoint mouse_position = from_app_to_canvas(render_area, mouse_pressed_position);

    if (render_area.hovers(event->pos()))
        render_area.decide_dragging( mouse_position );

}

void ControlLineWidget::mouseMoveEvent(QMouseEvent *event)
{
    if ( (mouse_pressed_position - event->pos()).manhattanLength() > 25)
        dragging = true;

    if (dragging){
        QPoint mouse_pos = from_app_to_canvas(render_area, event->pos());
        render_area.update_dragging(mouse_pos);
    }

    update();

}

void ControlLineWidget::mouseReleaseEvent(QMouseEvent *event)
{
    dragging = false;
}

bool ControlLineWidget::is_dragging() { return dragging; }

qreal ControlLineWidget::initial_width() { return INITIAL_WIDTH; }

qreal ControlLineWidget::initial_height() { return INITIAL_HEIGHT; }

ControlLineWidget::~ControlLineWidget() { ; }
