#include "controlboard.h"
#include "ui_controlboard.h"

#include <QtGui>

ControlBoard::ControlBoard(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ControlBoard)
{
    ui->setupUi(this);
    controlLineWidget = new ControlLineWidget(ui->frame_LineWidget);
}

ControlBoard::~ControlBoard()
{
    delete ui;
}

void ControlBoard::resizeEvent(QResizeEvent *)
{

    if ((int) controlLineWidget->initial_width() == 0 &&
        (int) controlLineWidget->initial_height() == 0)
       controlLineWidget->set_initial_dimensions(this->rect());

}
