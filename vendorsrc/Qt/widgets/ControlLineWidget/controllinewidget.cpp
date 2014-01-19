#include <QtGui>

#include "math.h"
#include "controllinewidget.h"
#include "plotdata.h"

#define RENDER_XMARGIN 60
#define RENDER_YMARGIN RENDER_XMARGIN
#define SELECTION_ERROR 0.01

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

const QVector<QString> &ControlLineWidget::get_string_of_control_points()
{
    QVector<QString> points;
    for (int i=0; i<control_points.points().size(); i++){
        QPointF point = *control_points.points().at(i);
        QString representation = "Point " + QString::number(i) + ": (";
        representation += QString::number(point.x()) + ", ";
        representation += QString::number(point.y()) + ")";
        points.append( representation) ;
    }
    return points;
}

void ControlLineWidget::broadcast_active_point_coords_changed(int point,
                                                              const PlotPoint &value)
{
    if (point == render_area.get_active_point())
        emit(active_point_coords_changed(value));
}

void ControlLineWidget::toggle_logarithmic(bool value)
{
    render_area.change_logarithmic(value);
    update();
}

void ControlLineWidget::toggle_active_point_x_lock(bool value)
{
    control_points.points()[render_area.get_active_point()]->set_x_lock(value);
}

void ControlLineWidget::toggle_active_point_y_lock(bool value)
{
    control_points.points()[render_area.get_active_point()]->set_y_lock(value);
}

void ControlLineWidget::change_active_point_x_value(double value)
{
    control_points.points()[render_area.get_active_point()]->setX(value);
    update();
}

void ControlLineWidget::change_active_point_y_value(double value)
{
    control_points.points()[render_area.get_active_point()]->setY(value);
    update();
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

void ControlLineWidget::insert_new_point(const QPoint mouse_position)
{
    int number_of_points = control_points.points().count();
    int knob_radius = render_area.get_knob_radius();

    for (int i=1; i<number_of_points; i++){
        QPointF middle_distance = mouse_position -
                render_area.to_dev(*control_points.points().at(i - 1));
        QPointF total_distance = render_area.to_dev(*control_points.points().at(i)) -
                render_area.to_dev(*control_points.points().at(i - 1));
        QPointF unit_distance = middle_distance / middle_distance.manhattanLength() -
                total_distance / total_distance.manhattanLength() ;


        if ( render_area.decide_dragging( mouse_position ) < 0 &&
                QRectF(-SELECTION_ERROR, -SELECTION_ERROR,
                    2 * SELECTION_ERROR,  2 * SELECTION_ERROR).contains(unit_distance))
            control_points.points().insert(i,
                new PlotPoint(render_area.to_logic(mouse_position)));

    }
}

void ControlLineWidget::mousePressEvent(QMouseEvent *event)
{

    mouse_pressed_position = event->pos();
    QPoint mouse_position = from_app_to_canvas(render_area, mouse_pressed_position);

    insert_new_point(mouse_position);

    if (render_area.hovers(event->pos())) {
        int active_point = render_area.decide_dragging( mouse_position );
        if (active_point >= 0 && active_point < control_points.points().count())
            emit active_point_changed( active_point,
                                  *control_points.points().at(active_point));
    }

}

void ControlLineWidget::mouseMoveEvent(QMouseEvent *event)
{
    if ( (mouse_pressed_position - event->pos()).manhattanLength() > 25)
        dragging = true;

    if (dragging && render_area.get_active_point() >= 0){
        QPoint mouse_pos = from_app_to_canvas(render_area, event->pos());
        if (control_points.points().count() > 2 ||
            render_area.get_visualization_data().sensitive_area().contains(mouse_pos)){
            render_area.update_dragging(mouse_pos);
            emit active_point_coords_changed(
                        *control_points.points().at( render_area.get_active_point())
                        );
        }
    }

    update();

}

void ControlLineWidget::mouseReleaseEvent(QMouseEvent *event) {
    bool out = false;
    QPoint mouse_pos = from_app_to_canvas(render_area, event->pos());
    dragging = false;
    if ( control_points.points().count() > 2 &&
         !render_area.get_visualization_data().sensitive_area().contains(mouse_pos)){
        out = true;
        control_points.remove(render_area.get_active_point());
        update();
    }
    render_area.stop_dragging(out);
}

bool ControlLineWidget::is_dragging() { return dragging; }
qreal ControlLineWidget::initial_width() const { return total_render_width; }
qreal ControlLineWidget::initial_height() const { return total_render_height; }

ControlLineWidget::~ControlLineWidget() { ; }
