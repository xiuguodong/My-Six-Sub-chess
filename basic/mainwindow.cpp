#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <gamewindow.h>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_bntpeopeo_clicked()
{
    this->close();
    GameWindow* GMPP = new GameWindow;
    GMPP->show();
}

void MainWindow::on_bntpeonet_clicked()
{
    this->close();
    GameWindow* GMPN = new GameWindow;
    GMPN->show();
}

void MainWindow::on_bntnetnet_clicked()
{
    this->close();
    GameWindow* GMNN = new GameWindow;
    GMNN->show();
}
