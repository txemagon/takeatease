#ifndef CONTROLLINEWIDGET_H
#define CONTROLLINEWIDGET_H

#include <QtGui/QWidget>


#define MID_POINT 1
#define SUB_DIV   2
#define MAIN_DIV  4
#define ALL_DIV   (MAIN_DIV | SUB_DIV)

class ControlLineWidget : public QWidget
{
    Q_OBJECT
    
public:
    ControlLineWidget(QWidget *parent = 0);
    ~ControlLineWidget();

    void mousePressEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);

protected:
    void paintEvent(QPaintEvent *event);

    void draw_horizontal_mid_div(QPainter &painter);
    void draw_horizontal_log_grid(QPainter &painter, int options = ALL_DIV);
    void draw_horizontal_grid(QPainter &painter, int options = ALL_DIV);
    void draw_vertical_grid(QPainter &painter, int options = ALL_DIV);

    void setup_canvas(QPainter &painter);

    int linear_limit(qreal range);
    QPointF to_dev(QPointF logical);
    QPointF to_dev(qreal x, qreal y);
    QPointF to_logic(QPointF device);
    QPointF to_logic(qreal x, qreal y);
    QPoint from_app_to_canvas(QPoint canvas);

    QPointF belongs_to(QPointF point, QVector<QPointF> line);

private:
    static const qreal x_range[2];
    static const qreal y_range[2];
    QVector<QPointF> control_points;
    bool logarithmic;
    QPoint mouse_pressed_pos;
    bool dragging;
    int active_point;

    qreal interval(const qreal range[2]);
    QPointF range_lower_limit();
    QPointF range_upper_limit();

    qreal knob_radius;

};

#endif // CONTROLLINEWIDGET_H
