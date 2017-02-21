#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "node.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    emit sig_SendNode(new Node("ROOT")); // Emit a signal to create an initial node in the tree widget
}

MainWindow::~MainWindow()
{
    delete ui;
}
