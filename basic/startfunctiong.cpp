#include "startfunctiong.h"
#include "gamewindow.h"
#include "checkfunction.h"
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

startfunctiong::startfunctiong()
{

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
