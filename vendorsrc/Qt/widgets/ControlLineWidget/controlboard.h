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
    
private:
    Ui::ControlBoard *ui;
    ControlLineWidget *controlLineWidget;
};

#endif // CONTROLBOARD_H
