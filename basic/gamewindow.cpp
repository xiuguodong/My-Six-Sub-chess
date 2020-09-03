#include "gamewindow.h"
#include "ui_gamewindow.h"
#include <QPalette>
#include <QMouseEvent>
#include <QPainter>
#include <QPen>
#include <QBrush>
#include <QTime>
#include <QPoint>
#include <QWidget>
#include <QDebug>

GameWindow::GameWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::GameWindow)
{
    init();
    ui->setupUi(this);

    this->setFixedSize(1060,680);
    this->setAutoFillBackground(true);
    QPalette palette;
    palette.setColor(QPalette::Background,QColor("#B1723C"));
    this->setPalette(palette);//设置背景色！
    centralWidget()->setMouseTracking(true);
    setMouseTracking(true);
    nextchess=0;
}

void GameWindow::init()
{
    nextchess=0;
    for (int i = 0; i < 20; i ++)
    {
        for (int j = 0; j < 20; j ++)
        {
            chess[i][j]=0;
        }
    }
    f[0][0]=0;f[0][1]=1;
    f[4][0]=0;f[4][1]=-1;
    f[1][0]=-1;f[1][1]=0;
    f[5][0]=1;f[5][1]=0;
    f[2][0]=1;f[2][1]=1;
    f[6][0]=-1;f[6][1]=-1;
    f[3][0]=-1;f[3][1]=1;
    f[7][0]=1;f[7][1]=-1;
}
void GameWindow::mouseMoveEvent(QMouseEvent *event)
{
    movex = (event->y() - 45 )/ 30;
    movey = (event->x() - 25 )/ 30;
    //qDebug() << event->x() << ' ' <<event->y();
    QWidget::update();
}

void GameWindow::search()
{
    for (int i = 0; i < 20; i ++)
    {
        for (int j = 0; j < 20 ; j ++)
        {
            for (int k = 0; k < 4; k ++)
            {
                chess_number[i][j][k]=0;
            }
        }
    }
    for (int i = 0; i < 20; i ++)
    {
        for (int j = 0; j < 20 ; j ++)
        {
            if (chess[i][j]!=0)
            {
                chess_number[i][j][0]=1;
                chess_number[i][j][1]=1;
                chess_number[i][j][2]=1;
                chess_number[i][j][3]=1;
                for (int k = 0; k < 8; k ++)
                {
                    for (int l = 1; l < 6; l ++)
                    {
                        int dx = i + f[k][0] * l;
                        int dy = j + f[k][1] * l;
                        if (dx<0||dy<0||dx>=20||dy>=20||
                           (chess[dx][dy]!=chess[i][j]&&chess[dx][dy]!=0))
                        {
                            break;
                        }
                        if (chess[dx][dy]==chess[i][j])
                        {
                            chess_number[i][j][k%4]++;
                            //qDebug()<<dx<<' '<<dx<<' '<<k%4<<' '<<chess_number[i][j][k%4];
                        }
                    }
                }
            }
        }
    }
}

int GameWindow::check_ok(int x,int y)
{
    if (x<0||x>=20||y<0||y>=20)
    {
        return 1;
    }
    search ();
    int temp4=0,temp5=0;
    for (int i = 0; i < 4; i ++)
    {
        if (chess_number[x][y][i] == 4)
        {
            temp4++;
        }
        if (chess_number[x][y][i] == 5)
        {
            temp5++;
        }
    }
    if (temp4 >= 2||temp5 >= 2)
    {
        return 3;
    }
    return 0;
}

bool GameWindow::check_victom(int x,int y)
{
    for (int i = 0; i < 4; i ++)
    if (chess_number[x][y][i] == 6)
        return 1;
    return 0;
}

void GameWindow::mousePressEvent(QMouseEvent *event)
{
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
        switch(check_ok(currentx,currenty))
        {
            case 0:break;
            case 1:qDebug() << "超出边界";
            chess[currentx][currenty] = 0;goto loop;
            case 3:qDebug() << "该点禁手";
            chess[currentx][currenty] = 0;goto loop;
        }
        QWidget::update();
        if (check_victom(currentx,currenty))
        {
            qDebug() << "黑棋胜";
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
        switch(check_ok(currentx,currenty))
        {
            case 0:break;
            case 1:qDebug() << "超出边界";
            chess[currentx][currenty] = 0;goto loop;
            case 3:qDebug() << "该点禁手";
            chess[currentx][currenty] = 0;goto loop;
        }
        QWidget::update();
        if (check_victom(currentx,currenty))
        {
            qDebug() << "白棋胜";
        }
        nextchess = 0;
    }
    loop:;
}

/*void GameWindow::mousePressEvent(QMouseEvent *event)
{
    currentx = (event->y() - 45)/ 30;
    currenty = (event->x() - 25)/ 30;
    if (nextchess==0)
    {
        chess[currentx][currenty]=4;
        nextchess=1;
    }
    else
    {
        chess[currentx][currenty]=5;
        nextchess=0;
    }
    update();
}
*/
void GameWindow::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing,true);

    QPen pen = painter.pen();
    pen.setColor(QColor("#8D5822"));
    pen.setWidth(7);
    painter.setPen(pen);

    QBrush brush;
    brush.setColor(QColor("#EEC085"));
    brush.setStyle(Qt::SolidPattern);
    painter.setBrush(brush);
    painter.drawRect(20,30,610,620);

    pen.setColor(Qt::black);
    pen.setWidth(1);
    painter.setPen(pen);
    for (int i = 0; i < 20 ; i ++ )
    {
        painter.drawLine(40 + i * 30, 60, 40 + i * 30, 630);
        painter.drawLine(40, 60 + i * 30, 610, 60 + i * 30);
    }

    /*
    qsrand(time(0));
    for (int i = 0; i < 20; i ++)
    {
        for (int j = 0; j < 20; j ++)
        {
            chess[i][j] = 4;
        }
    }
    */

    for (int i = 0; i < 20; i ++)
    {
        for (int j = 0;j < 20; j ++)
        {
            if (chess[i][j] == 4)
            {
                brush.setColor(Qt::black);
                painter.setBrush(brush);
                painter.drawEllipse(QPoint((j + 1)* 30 + 10,(i + 1)* 30 + 30)
                                    ,14,14);
            }
            if (chess[i][j] == 5)
            {
                brush.setColor(Qt::white);
                painter.setPen(Qt::NoPen);
                painter.setBrush(brush);
                painter.drawEllipse(QPoint((j + 1)* 30 + 10,(i + 1)* 30 + 30)
                                    ,14,14);
            }
        }
    }

    pen.setColor(Qt::red);
    pen.setWidth(1);
    painter.setPen(pen);
    if ((movex * 30 + 30) >= 20 && (movex * 30 + 30) <= 620
            && (movey * 30 + 30) >= 0 && (movey * 30 + 30) <= 600)
    {
        painter.drawLine((movey + 1)* 30 - 5,(movex + 1)* 30 + 15,
                         (movey + 1)* 30 + 5,(movex + 1)* 30 + 15);
        painter.drawLine((movey + 1)* 30 + 25,(movex + 1)* 30 + 15,
                         (movey + 1)* 30 + 15,(movex + 1)* 30 + 15);
        painter.drawLine((movey + 1)* 30 - 5,(movex + 1)* 30 + 45,
                         (movey + 1)* 30 + 5,(movex + 1)* 30 + 45);
        painter.drawLine((movey + 1)* 30 + 25,(movex + 1)* 30 + 45,
                         (movey + 1)* 30 + 15,(movex + 1)* 30 + 45);
        painter.drawLine((movey + 1)* 30 - 5,(movex + 1)* 30 + 15,
                         (movey + 1)* 30 - 5,(movex + 1)* 30 + 25);
        painter.drawLine((movey + 1)* 30 + 25,(movex + 1)* 30 + 15,
                         (movey + 1)* 30 + 25,(movex + 1)* 30 + 25);
        painter.drawLine((movey + 1)* 30 - 5,(movex + 1)* 30 + 45,
                         (movey + 1)* 30 - 5,(movex + 1)* 30 + 35);
        painter.drawLine((movey + 1)* 30 + 25,(movex + 1)* 30 + 45,
                         (movey + 1)* 30 + 25,(movex + 1)* 30 + 35);
    }

    painter.drawLine((currenty + 1)* 30 - 1+10,(currentx + 1)* 30 + 20+10,
                     (currenty + 1)* 30 - 6+10,(currentx + 1)* 30 + 20+10);
    painter.drawLine((currenty + 1)* 30 + 1+10,(currentx + 1)* 30 + 20+10,
                     (currenty + 1)* 30 + 6+10,(currentx + 1)* 30 + 20+10);
    painter.drawLine((currenty + 1)* 30+10,(currentx + 1)* 30 + 19+10,
                     (currenty + 1)* 30+10,(currentx + 1)* 30 + 14+10);
    painter.drawLine((currenty + 1)* 30+10,(currentx + 1)* 30 + 21+10,
                     (currenty + 1)* 30+10,(currentx + 1)* 30 + 26+10);
}

GameWindow::~GameWindow()
{
    delete ui;
}
