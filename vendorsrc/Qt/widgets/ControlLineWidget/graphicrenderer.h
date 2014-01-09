#ifndef GRAPHICRENDERER_H
#define GRAPHICRENDERER_H

#include "plotdata.h"
#include "visualizationdata.h"

#define LOGARITHMIC_AXIS true

#define MID_POINT 1
#define SUB_DIV   2
#define MAIN_DIV  4
#define ALL_DIV   (MAIN_DIV | SUB_DIV)

class VisualizationData;

class GraphicRenderer
{
public:
    GraphicRenderer();
    GraphicRenderer(QWidget *parent,
                    PlotData *plot_points,
                    VisualizationData visual_data,
                    qreal knob_radius,
                    bool logarithmic_axis);

    void set_coords(QPainter &painter);
    void setup_canvas(QPainter &painter);
    void paint(QPainter &painter);

    bool hovers(const QPoint &mouse_abs_pos);
    int decide_dragging(const QPoint &mouse_pos);
    void update_dragging(const QPoint &mouse_now);
    void stop_dragging();
    VisualizationData get_visualization_data();
    int get_active_point();

protected:
    int linear_limit(qreal range);
    QPointF to_dev(QPointF logical);
    QPointF to_dev(QPointF *logical);
    QPointF to_dev(qreal x, qreal y);
    QPointF to_logic(QPointF device);
    QPointF to_logic(QPointF *device);
    QPointF to_logic(qreal x, qreal y);

    void draw_horizontal_mid_div(QPainter &painter);
    void draw_horizontal_log_grid(QPainter &painter, int options = ALL_DIV);
    void draw_horizontal_grid(QPainter &painter, int options = ALL_DIV);
    void draw_vertical_grid(QPainter &painter, int options = ALL_DIV);

private:

    static const qreal x_range[2];
    static const qreal y_range[2];
    PlotData *plot_points;
    VisualizationData visual_data;

    bool logarithmic;
    qreal knob_radius;

    QPoint mouse_pressed_pos;
    int active_point;
    bool dragging;

    const QWidget *parent;

    qreal interval(const qreal range[2]);
    QPointF range_lower_limit();
    QPointF range_upper_limit();

};

#endif // GRAPHICRENDERER_H
