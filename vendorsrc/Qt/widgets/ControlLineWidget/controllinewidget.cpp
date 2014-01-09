#include <QtGui>

#include "math.h"
#include "controllinewidget.h"
#include "plotdata.h"

#define RENDER_XMARGIN 60
#define RENDER_YMARGIN RENDER_XMARGIN

ControlLineWidget::ControlLineWidget(QWidget *parent)
    : QWidget(parent)
{
    container = parent;
    width0    = 0;
    height0   = 0;

    knob_radius  =  7;

    mouse_pressed_position = QPoint(-1, -1);
    dragging     = false;

    control_points.points() <<
                      new PlotPoint(16., .7, true) <<
                      new PlotPoint(3000., 1.) <<
                      new PlotPoint(25000., 1.20, true);

    connect(&control_points, SIGNAL(coordinates_changed(int, const PlotPoint &)),
            this, SLOT(broadcast_active_point_coords_changed(int, const PlotPoint &)));
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
        painter.scale(container->width()  / total_render_width,
                      container->height() / total_render_height);

        // Black background rectangle
        QLinearGradient gradient(QPointF(0., 0.),
                                 QPointF(total_render_width, total_render_height));
        gradient.setColorAt(0, Qt::black);
        gradient.setColorAt(1, Qt::gray);
        painter.fillRect(0, 0, total_render_width, total_render_height, gradient);

        safety_paint(painter, render_area, &GraphicRenderer::setup_canvas);
}

void ControlLineWidget::set_initial_dimensions(QRect dimension)
{
    if ( !(int) total_render_width &&
         !(int) total_render_height) {

        total_render_width = dimension.width();
        width0 = total_render_width - 2 * RENDER_XMARGIN;
        total_render_height = dimension.height();
        height0 = total_render_height - 2 * RENDER_YMARGIN;

        render_area = GraphicRenderer (this,
                                    &control_points,
                                    VisualizationData( RENDER_XMARGIN / 2,
                                                       RENDER_YMARGIN / 2,
                                                       total_render_width  - RENDER_XMARGIN,
                                                       total_render_height - RENDER_YMARGIN,
                                                       RENDER_XMARGIN / 2,
                                                       RENDER_YMARGIN / 2, true),
                                    knob_radius, LOGARITHMIC_AXIS);
        resize(total_render_width, total_render_height);
    }

}

void ControlLineWidget::broadcast_active_point_coords_changed(int point,
                                                              const PlotPoint &value)
{
    if (point == render_area.get_active_point())
        emit(active_point_coords_changed(value));
}

QPointF &ControlLineWidget::get_active_point()
{
    return *control_points.points()[render_area.get_active_point()];
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


    absolute_point.setX(absolute_point.x() * initial_width()  / container->width());
    absolute_point.setY(absolute_point.y() * initial_height() / container->height());
    QPoint corner = absolute_point.toPoint() - top;

    if (visual.inverted_y_coordinate)
        corner.setY(visual.total_height() - corner.y());

    return corner;
}

void ControlLineWidget::mousePressEvent(QMouseEvent *event)
{

    mouse_pressed_position = event->pos();
    QPoint mouse_position = from_app_to_canvas(render_area, mouse_pressed_position);

    if (render_area.hovers(event->pos())) {
        int active_point = render_area.decide_dragging( mouse_position );
        emit active_point_changed( active_point,
                                  *control_points.points().at(active_point));
    }

}

void ControlLineWidget::mouseMoveEvent(QMouseEvent *event)
{
    if ( (mouse_pressed_position - event->pos()).manhattanLength() > 25)
        dragging = true;

    if (dragging){
        QPoint mouse_pos = from_app_to_canvas(render_area, event->pos());
        render_area.update_dragging(mouse_pos);
        emit active_point_coords_changed(
                    *control_points.points().at( render_area.get_active_point())
                    );
    }

    update();

}

void ControlLineWidget::mouseReleaseEvent(QMouseEvent *) { dragging = false; }
bool ControlLineWidget::is_dragging() { return dragging; }
qreal ControlLineWidget::initial_width() const { return total_render_width; }
qreal ControlLineWidget::initial_height() const { return total_render_height; }

ControlLineWidget::~ControlLineWidget() { ; }
