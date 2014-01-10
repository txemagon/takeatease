#include "controlboard.h"
#include "ui_controlboard.h"

#include <QtGui>

ControlBoard::ControlBoard(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ControlBoard)
{
    ui->setupUi(this);
    controlLineWidget = new ControlLineWidget(ui->frame_LineWidget);

    /* incoming signals */
    connect(controlLineWidget, SIGNAL(active_point_changed(int, const PlotPoint &)),
            this, SLOT(change_in_active_point(int, const PlotPoint &)));
    connect(controlLineWidget, SIGNAL(active_point_coords_changed(const PlotPoint &)),
            this, SLOT(change_in_active_point(const PlotPoint &)));

    /* outgoing signals */
    connect(ui->checkBox_Logarithmic, SIGNAL(toggled(bool)),
            controlLineWidget, SLOT(toggle_logarithmic(bool)));
    connect(ui->checkBox_XLock, SIGNAL(toggled(bool)),
            controlLineWidget, SLOT(toggle_active_point_x_lock(bool)));
    connect(ui->checkBox_YLock, SIGNAL(toggled(bool)),
            controlLineWidget, SLOT(toggle_active_point_y_lock(bool)));
}

ControlBoard::~ControlBoard()
{
    delete controlLineWidget;
    delete ui;
}

void ControlBoard::resizeEvent(QResizeEvent *)
{

    if ((int) controlLineWidget->initial_width() == 0 &&
        (int) controlLineWidget->initial_height() == 0)
        controlLineWidget->set_initial_dimensions(this->rect());

}

void ControlBoard::change_in_active_point(int active_point, const PlotPoint &point)
{
    QGroupBox *groupbox_CurrentPoint = ui->groupBox_CurrentPoint;
    QString title = "Current Point ";

    if (active_point < 0)
        groupbox_CurrentPoint->setEnabled(false);
    else {
        groupbox_CurrentPoint->setEnabled(true);
        title.append(QString::number(active_point + 1));

        ui->doubleSpinBox_XPoint->setValue(point.x());
        ui->doubleSpinBox_YPoint->setValue(point.y());
        ui->checkBox_XLock->setChecked(point.x_lock());
        ui->checkBox_YLock->setChecked(point.y_lock());
    }
    groupbox_CurrentPoint->setTitle(title);
    update();
}

void ControlBoard::change_in_active_point(const PlotPoint &coords)
{
    ui->doubleSpinBox_XPoint->setValue(coords.x());
    ui->doubleSpinBox_YPoint->setValue(coords.y());
}
