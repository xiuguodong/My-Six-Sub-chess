#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "gamewindow.h"
//#include "gamewindoweve.h"
#include "gamewindowpve.h"
#include <QDebug>

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
    GameWindowPVE* GMPN = new GameWindowPVE;
    GMPN->show();
    qDebug()<<1;
}

/*void MainWindow::on_bntnetnet_clicked()
{
    this->close();
    GameWindowEVE* GMNN = new GameWindowEVE;
    GMNN->show();
}
*/
