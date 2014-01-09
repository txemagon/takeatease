#ifndef CONTROLBOARD_H
#define CONTROLBOARD_H

#include <QWidget>
#include "controllinewidget.h"

namespace Ui {
   class ControlBoard;
}

class ControlBoard : public QWidget
{
    Q_OBJECT
    
public:
    explicit ControlBoard(QWidget *parent = 0);
    ~ControlBoard();
    void resizeEvent(QResizeEvent *);

public slots:
    void change_in_active_point(int active_point, const PlotPoint &point);
    void change_in_active_point(const PlotPoint &coords);

private:
    Ui::ControlBoard *ui;
    ControlLineWidget *controlLineWidget;
};

#endif // CONTROLBOARD_H
