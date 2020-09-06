#include "gamewindow.h"
#include "ui_gamewindow.h"
#include "checkfunction.h"
#include "startfunctiong.h"
#include <QPalette>
#include <QMouseEvent>
#include <QPainter>
#include <QPen>
#include <QBrush>
#include <QTime>
#include <QPoint>
#include <QWidget>
#include <QDebug>
#include <QLabel>
#include <QMessageBox>
#include <QAction>

GameWindow::GameWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::GameWindow)
{
    init();
    ui->setupUi(this);

    this->setFixedSize(660,680);
    this->setAutoFillBackground(true);
    QPalette palette;
    palette.setColor(QPalette::Background,QColor("#B1723C"));
    this->setPalette(palette);//设置背景色！
    centralWidget()->setMouseTracking(true);
    setMouseTracking(true);
    nextchess=0;
}

void GameWindow::mouseMoveEvent(QMouseEvent *event)
{
    movex = (event->y() - 45 )/ 30;
    movey = (event->x() - 25 )/ 30;
    //qDebug() << event->x() << ' ' <<event->y();
    QWidget::update();
}



void GameWindow::mousePressEvent(QMouseEvent *event)
{
    int flag = 0;
    if (nextchess==0)
    {
        currentx = (event->y() - 45)/ 30;
        currenty = (event->x() - 25)/ 30;
        if (chess[currentx][currenty] != 0)
        {
            qDebug() << "该点已被下";
            goto loop;
        }
        chess[currentx][currenty] = 4;
        alreadydone++;
        switch(check_ok(currentx,currenty))
        {
            case 0:break;
            case 1:qDebug() << "超出边界";
            chess[currentx][currenty] = 0,alreadydone--;goto loop;
            case 2:qDebug() << "五五禁手";
            chess[currentx][currenty] = 0;flag=-1;goto loop;
            case 3:qDebug() << "四四禁手";
            chess[currentx][currenty] = 0;flag=-1;goto loop;
        }
        QWidget::update();
        if (check_victom(currentx,currenty))
        {
            qDebug() << "黑棋胜";flag=1;
        }
        nextchess = 1;
    }
    else
    {
        currentx = (event->y() - 45)/ 30;
        currenty = (event->x() - 25)/ 30;
        if (chess[currentx][currenty] != 0)
        {
            qDebug() << "该点已被下";
            goto loop;
        }
        chess[currentx][currenty] = 5;
        alreadydone++;
        switch(check_ok(currentx,currenty))
        {
            case 0:break;
            case 1:qDebug() << "超出边界";
            chess[currentx][currenty] = 0;alreadydone--;goto loop;
            //白棋无禁手
        }
        QWidget::update();
        if (check_victom(currentx,currenty))
        {
            qDebug() << "白棋胜";flag=-1;
        }
        nextchess = 0;
    }
    loop:;

    if (flag == 1)
    {
        int ret = QMessageBox::question(this,"比赛结束","黑棋胜利",
                              QMessageBox::Reset,QMessageBox::Close);
        switch (ret)
        {
        case QMessageBox::Reset:
            init();
            break;
        case QMessageBox::Close:
            this->close();
            break;
        default:
            break;
        }
    }
    if (flag == -1)
    {
        int ret = QMessageBox::question(this,"比赛结束","白棋胜利",
                              QMessageBox::Reset,QMessageBox::Close);
        switch (ret)
        {
        case QMessageBox::Reset:
            init();
            break;
        case QMessageBox::Close:
            this->close();
            break;
        default:
            break;
        }
    }
    if (alreadydone == 400)
    {
        int ret = QMessageBox::question(this,"比赛结束","和棋",
                              QMessageBox::Reset,QMessageBox::Close);
        switch (ret)
        {
        case QMessageBox::Reset:
            init();
            break;
        case QMessageBox::Close:
            this->close();
            break;
        default:
            break;
        }
    }
}

GameWindow::~GameWindow()
{
    delete ui;
}
