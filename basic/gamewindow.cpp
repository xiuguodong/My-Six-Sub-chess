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

void GameWindow::mouseMoveEvent(QMouseEvent *event)//!!!!
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
                for (int k = 0; k < 4; k ++)
                {
                    for (int p = -5; p <= 0; p ++)
                    {
                        int number = 0;
                        int dx = i + f[k][0] * p;
                        int dy = j + f[k][1] * p;
                        if (dx<0||dy<0||dx>=20||dy>=20)continue;
                        //
                        for (int l = 0; l < 6; l ++)
                        {
                            if (dx<0||dy<0||dx>=20||dy>=20)break;
                            if (chess[dx][dy] == chess[i][j])
                                number++;
                            if (chess[dx][dy] == 9 - chess[i][j])
                            {
                                if (l + p < 0)
                                    number = 0;
                                break;
                            }
                            //if (k==0)qDebug() << dx <<' '<< dy;
                            dx += f[k][0];
                            dy += f[k][1];
                        }
                        if (number > chess_number[i][j][k])
                        {
                            chess_number[i][j][k]=number;
                        }
                    }
                    //qDebug() <<i<<' '<<j<<' '<<k<<' '<<chess_number[i][j][k];

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
    //qDebug() << chess_number[x][y][0]<<' '<<chess_number[x][y][1]<<' '
    //         <<chess_number[x][y][2]<<' ' <<chess_number[x][y][3];
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
    if (nextchess == 0&&temp5 >= 2)
    {
        return 2;
    }
    if (nextchess == 0&&temp4 >= 2)
    {
        int temp = 0;
        for (int i = 0; i < 4; i ++)
        {
            if (chess_number[x][y][i] == 4)
            {
                int mark = 0;//1->不连续点数目，2->两侧空出点数目
                //1的值大于1不禁手，等于1但2的值小于1不禁手，等于0但2的值小于2不禁手
                for (int j = -5; j <= 0; j ++)
                {
                    int dx = x + f[i][0] * j;
                    int dy = y + f[i][1] * j;
                    if (dx<0||dy<0||dx>=20||dy>=20) continue;
                    int flag = 0,ok = 1;
                    for (int k = 0; k < 6; k++)
                    {
                        if (dx<0||dy<0||dx>=20||dy>=20) break;
                        if (chess[dx][dy] == chess[x][y])
                            flag ++;
                        if (chess[dx][dy] == 9 - chess[x][y])
                            ok = 0;
                        dx += f[i][0];
                        dy += f[i][1];
                    }
                    if (ok==0||flag<4||dx<0||dy<0||dx>=20||dy>=20)
                        continue;
                    if (chess[dx-f[i][0]][dy-f[i][1]] == chess[x][y]&&
                        chess[dx-2*f[i][0]][dy-2*f[i][1]] == chess[x][y]
                            &&chess[dx][dy] == 0)
                        ok = 2;
                    dx = x + f[i][0] * j;
                    dy = y + f[i][1] * j;
                    if (ok == 2||(chess[dx][dy] == chess[x][y]&&
                        chess[dx+f[i][0]][dy+f[i][1]] == chess[x][y]&&
                        dx - f[i][0] >= 0&&dy - f[i][1] >= 0&&
                        dx - f[i][0] < 20&&dy - f[i][1] < 20&&
                        chess[dx-f[i][0]][dy-f[i][1]] == 0))
                            ok = 2;
                    if (ok != 2) continue;
                    flag = 0;ok = 0;
                    for (int k = 0; k < 6; k ++)
                    {
                        if (chess[dx][dy] == chess[x][y])
                            flag ++;
                        if (flag > 0&&flag < 4&& chess[dx][dy] == 0)
                            ok ++;
                        dx += f[i][0];
                        dy += f[i][1];
                    }
                    if (ok > 1) continue;
                    mark = 1;
                }
                if (mark == 1)
                    temp ++;
            }
        }
        if (temp >= 2)
            return 3;
    }
    return 0;
}

bool GameWindow::check_victom(int x,int y)
{
    for (int i = 0; i < 4; i ++)
    if (chess_number[x][y][i] >= 6)
    {
        return 1;
    }

    return 0;
}

void GameWindow::init()
{
    nextchess=0,alreadydone=0;
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
