#include "controlboard.h"
#include "ui_controlboard.h"

#include <QtGui>

ControlBoard::ControlBoard(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ControlBoard)
{
    ui->setupUi(this);
    controlLineWidget = new ControlLineWidget(ui->frame_LineWidget);

    connect(controlLineWidget, SIGNAL(active_point_changed(int)),
            this, SLOT(change_in_active_point(int)));
    connect(controlLineWidget, SIGNAL(active_point_coords_changed(QPointF)),
            this, SLOT(change_in_active_point(QPointF)));
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

void ControlBoard::change_in_active_point(int active_point)
{
    QGroupBox *groupbox_CurrentPoint = ui->groupBox_CurrentPoint;
    QString title = "Current Point ";

    if (active_point < 0)
        groupbox_CurrentPoint->setEnabled(false);
    else {
        groupbox_CurrentPoint->setEnabled(true);
        title.append(QString::number(active_point + 1));
    }
    groupbox_CurrentPoint->setTitle(title);
    update();
}

void ControlBoard::change_in_active_point(QPointF coords)
{
    ui->doubleSpinBox_XPoint->setValue(coords.x());
    ui->doubleSpinBox_YPoint->setValue(coords.y());
}
