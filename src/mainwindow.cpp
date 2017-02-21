#include "mainwindow.h"
#include <ui_mainwindow.h>
#include "cameracontrolshelp.h"
#include <QString>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->label->hide();
    ui->label_2->hide();
    ui->label_3->hide();
    ui->label_4->hide();
    ui->label_5->hide();
    ui->label_6->hide();
    ui->label_7->hide();
    ui->label_8->hide();

    ui->mygl->setFocus();

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionQuit_triggered()
{
    QApplication::exit();
}

void MainWindow::on_actionCamera_Controls_triggered()
{
    CameraControlsHelp* c = new CameraControlsHelp();
    c->show();
}
void MainWindow::slot_show_BlockNum()
{
    //std::cout<<"show "<<std::endl;
    ui->label->setText(QString::number(ui->mygl->geom_inventory.NumList[0]));
    ui->label_2->setText(QString::number(ui->mygl->geom_inventory.NumList[1]));
    ui->label_3->setText(QString::number(ui->mygl->geom_inventory.NumList[2]));
    ui->label_4->setText(QString::number(ui->mygl->geom_inventory.NumList[3]));
    ui->label_5->setText(QString::number(ui->mygl->geom_inventory.NumList[4]));
    ui->label_6->setText(QString::number(ui->mygl->geom_inventory.NumList[5]));
    ui->label_7->setText(QString::number(ui->mygl->geom_inventory.NumList[6]));
    ui->label_8->setText(QString::number(ui->mygl->geom_inventory.NumList[7]));
    ui->label->show();
    ui->label_2->show();
    ui->label_3->show();
    ui->label_4->show();
    ui->label_5->show();
    ui->label_6->show();
    ui->label_7->show();
    ui->label_8->show();
}
void MainWindow::slot_hide_BlockNum()
{
    //std::cout<<"hide "<<std::endl;
    ui->label->hide();
    ui->label_2->hide();
    ui->label_3->hide();
    ui->label_4->hide();
    ui->label_5->hide();
    ui->label_6->hide();
    ui->label_7->hide();
    ui->label_8->hide();
}
void MainWindow::slot_update_BlockNum()
{
    //std::cout<<"update "<<std::endl;
    ui->label->setText(QString::number(ui->mygl->geom_inventory.NumList[0]));
    ui->label_2->setText(QString::number(ui->mygl->geom_inventory.NumList[1]));
    ui->label_3->setText(QString::number(ui->mygl->geom_inventory.NumList[2]));
    ui->label_4->setText(QString::number(ui->mygl->geom_inventory.NumList[3]));
    ui->label_5->setText(QString::number(ui->mygl->geom_inventory.NumList[4]));
    ui->label_6->setText(QString::number(ui->mygl->geom_inventory.NumList[5]));
    ui->label_7->setText(QString::number(ui->mygl->geom_inventory.NumList[6]));
    ui->label_8->setText(QString::number(ui->mygl->geom_inventory.NumList[7]));
}
