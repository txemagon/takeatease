#ifndef PLOTPOINT_H
#define PLOTPOINT_H

#include <QObject>
#include <QPointF>

class PlotPoint : public QObject, public QPointF
{
    Q_OBJECT

public:
    explicit PlotPoint(QObject *parent = 0);
    explicit PlotPoint(const QPointF &point,
                       QObject *parent = 0);
    explicit PlotPoint(qreal x, qreal y,
                       bool x_lock=false, bool y_lock=false,
                       QObject *parent = 0);
    void setX(qreal x);
    void setY(qreal y);
    PlotPoint & set_coords(QPointF point);
    PlotPoint & operator+= ( const QPointF & point);
    PlotPoint & operator-= ( const QPointF & point);
    PlotPoint & operator*= ( qreal factor  );
    PlotPoint &	operator/= ( qreal divisor );

    bool x_lock() const;
    bool y_lock() const;

    void set_x_lock(bool value);
    void set_y_lock(bool value);

signals:
    void cordinates_changed(QPointF);
    
public slots:

private:
    bool locked[2];
};

#endif // PLOTPOINT_H
