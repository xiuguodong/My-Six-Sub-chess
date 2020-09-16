#include "serverwidget.h"
#include "ui_serverwidget.h"
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
#include<QTime>

ServerWidget::ServerWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ServerWidget)
{
    init();
    ui->setupUi(this);
    isStart = true;
    this->setFixedSize(660,680);
    this->setAutoFillBackground(true);
    QPalette palette;
    palette.setColor(QPalette::Background,QColor("#B1723C"));
    this->setPalette(palette);//设置背景色！
    setMouseTracking(true);
    tcpserver = NULL;
    tcpsocket =NULL;    //监听套接字
    tcpserver = new QTcpServer(this);
    tcpserver->listen(QHostAddress::Any,8888);
    setWindowTitle("服务器:8888");  //取窗口名字
    tcpsocket = new QTcpSocket(this);//自动回收空间
    connect(tcpserver,&QTcpServer::newConnection,
            [=]()
    {

        //取出建立好的套接字
        tcpsocket = tcpserver->nextPendingConnection();
        //显示当前连接对象,获取对方的IP和端口
        QString ip = tcpsocket->peerAddress().toString();
        qint16 prot = tcpsocket->peerPort();
        //组包，自定义格式显示
        QString temp = QString("[%1:%2]:成功连接").arg(ip).arg(prot);
        qDebug()<<temp;
        //显示对方发来的消息
        connect(tcpsocket,&QTcpSocket::disconnected,
                [=]()
                {
                    QTime time;
                    time.start();
                    while(time.elapsed() < 1500)
                        QCoreApplication::processEvents();
                    this->close();
                }
                );
        connect(tcpsocket,&QTcpSocket::readyRead,
                [=]()
        {
            //从通信套接字中取出内容
            QByteArray array =  tcpsocket->readAll();
            qDebug()<<array;
            waitforAi = 1;
            if (isStart == true)
            {
                peofirst=QString(array).section("##",0,0).toInt();
                startai=QString(array).section("##",1,1).toInt();
                qDebug()<<peofirst<<' '<<startai<<' '<<array;
                if (peofirst == 0&&startai == 1)
                {
                    actionByAi();
                    update();
                }
                if (peofirst == 0&&startai == 0)
                    waitforpeo = 1;
                if (peofirst == 1)
                    waitforpeo = 0;
                isStart=false;
            }
            else
            {
                for (int i = 0; i < 20; i ++)
                {
                    for (int j = 0; j < 20; j ++)
                    {
                        chess[i][j]=array[(i*20+j)*4];
                    }
                }
                flag=0;alreadydone=0;waitforpeo=1;
                for (int i = 0; i < 20; i ++)
                {
                    for (int j = 0; j < 20; j ++)
                    {
                        switch(check_ok(i,j))
                        {
                            case 0:break;
                            case 1:qDebug() << "超出边界";
                            chess[currentx][currenty] = 0,alreadydone--;goto loop;
                            case 2:qDebug() << "五五禁手";
                            chess[currentx][currenty] = 0;flag=-1;goto loop;
                            case 3:qDebug() << "四四禁手";
                            chess[currentx][currenty] = 0;flag=-1;goto loop;
                        }
                        //QWidget::update();
                        if (check_victom(i,j)&&chess[i][j]==4)
                        {
                            qDebug() << "黑棋胜";flag=1;goto loop;
                        }
                        if (check_victom(i,j)&&chess[i][j]==5)
                        {
                            qDebug() << "白棋胜";flag=-1;goto loop;
                        }
                        if (chess[i][j] != 0)
                            alreadydone++;
                    }
                }
                loop:;
                if (flag == 0&&startai == 1&&alreadydone != 400)
                    actionByAi();
                if (flag != 0||alreadydone == 400)
                {
                    QString str = QString ("%1##%2##-20##-20")
                            .arg(flag).arg(alreadydone);
                    qDebug()<<"here  "<<str;
                    QTime time;
                    time.start();
                    while(time.elapsed() < 300)
                        QCoreApplication::processEvents();
                        tcpsocket->write(str.toUtf8().data());
                        timer1.stop();
                }

            }
        }
        );
    }
    );

}

void ServerWidget::mouseMoveEvent(QMouseEvent *event)//取鼠标
{
    movex = (event->y() - 45 )/ 30;
    movey = (event->x() - 25 )/ 30;
    //qDebug() << event->x() << ' ' <<event->y();
    QWidget::update();
}

void ServerWidget::mousePressEvent(QMouseEvent *event)//人走子
{
    if (waitforpeo == 0||startai == 1)
    {
        return ;
    }
    waitforpeo = 0;
    if (peofirst==0)
    {
        //qDebug()<<"here";
        currentx = (event->y() - 45)/ 30;
        currenty = (event->x() - 25)/ 30;
        if (currentx<0||currentx>=20||currenty<0||currenty>=20)
        {
            qDebug() << "超出边界";
            return ;
        }
        if (chess[currentx][currenty] != 0)
        {
            qDebug() << "该点已被下";
            return ;
        }
        chess[currentx][currenty] = 4;
    }
    else
    {
        currentx = (event->y() - 45)/ 30;
        currenty = (event->x() - 25)/ 30;
        if (currentx<0||currentx>=20||currenty<0||currenty>=20)
        {
            qDebug() << "超出边界";
            return ;
        }
        if (chess[currentx][currenty] != 0)
        {
            qDebug() << "该点已被下";
            return ;
        }
        chess[currentx][currenty] = 5;
    }
    update();
    int flag=0;alreadydone=0;
    for (int i = 0; i < 20; i ++)
    {
        for (int j = 0; j < 20; j ++)
        {
            switch(check_ok(i,j))
            {
                case 0:break;
                case 1:qDebug() << "超出边界";
                chess[currentx][currenty] = 0,alreadydone--;goto loop;
                case 2:qDebug() << "五五禁手";
                chess[currentx][currenty] = 0;flag=-1;goto loop;
                case 3:qDebug() << "四四禁手";
                chess[currentx][currenty] = 0;flag=-1;goto loop;
            }
            //QWidget::update();
            if (check_victom(i,j)&&chess[i][j]==4)
            {
                qDebug() << "黑棋胜";flag=1;goto loop;
            }
            if (check_victom(i,j)&&chess[i][j]==5)
            {
                qDebug() << "白棋胜";flag=-1;goto loop;
            }
            if (chess[i][j] != 0)
                alreadydone++;
        }
    }
    loop:;
    QString str = QString ("%1##%2##%3##%4")
            .arg(flag).arg(alreadydone)
            .arg(currentx).arg(currenty);
    //qDebug()<<str;
    QTime time;
    time.start();
    while(time.elapsed() < 300)
        QCoreApplication::processEvents();
        tcpsocket->write(str.toUtf8().data());
        timer1.stop();
}


void ServerWidget::actionByAi()
{
    alreadydone = 0;
    for (int i = 0;i < 20; i ++)
    {
        for (int j = 0;j < 20; j ++)
        {
            if (chess[i][j]!=0)
                alreadydone++;
        }
    }
    if (alreadydone == 0)
    {
        chess[10][10] = 4 + peofirst;
        waitforAi = 0;
        QString str = QString ("0##1##10##10");
        tcpsocket->write(str.toUtf8().data());
        QTime time;
        time.start();
        while(time.elapsed() < 100)
            QCoreApplication::processEvents();
            tcpsocket->write(str.toUtf8().data());
            timer1.stop();

        time.start();
        while(time.elapsed() < 300)
            QCoreApplication::processEvents();
            tcpsocket->write((char*)chess,sizeof chess);
            timer.stop();
        return ;
    }
    int final_socer = -0x3f3f3f3f,final_place = -1;
    aiGetSocer();
    for (int i = 0; i < 20; i ++)
    {
        for (int j = 0; j < 20; j ++)
        {
            if (chess[i][j] == 0)
            {
                 if (final_socer < source[i][j])
                 {
                     final_socer = source[i][j];
                     final_place = i * 20 + j;
                 }
            }
        }
    }
    chess[final_place/20][final_place%20] = 4 + peofirst;
    qDebug() << final_place/20 <<' '<<final_place%20;
    currentx = final_place/20;
    currenty = final_place%20;
    waitforAi = 0;
    update();
    int flag=0;alreadydone=0;
    for (int i = 0; i < 20; i ++)
    {
        for (int j = 0; j < 20; j ++)
        {
            switch(check_ok(i,j))
            {
                case 0:break;
                case 1:qDebug() << "超出边界";
                chess[currentx][currenty] = 0,alreadydone--;goto loop;
                case 2:qDebug() << "五五禁手";
                chess[currentx][currenty] = 0;flag=-1;goto loop;
                case 3:qDebug() << "四四禁手";
                chess[currentx][currenty] = 0;flag=-1;goto loop;
            }
            //QWidget::update();
            if (check_victom(i,j)&&chess[i][j]==4)
            {
                qDebug() << "黑棋胜";flag=1;goto loop;
            }
            if (check_victom(i,j)&&chess[i][j]==5)
            {
                qDebug() << "白棋胜";flag=-1;goto loop;
            }
            if (chess[i][j] != 0)
                alreadydone++;
        }
    }
    loop:;
    QString str = QString ("%1##%2##%3##%4")
            .arg(flag).arg(alreadydone)
            .arg(final_place/20).arg(final_place%20);
    QTime time;
    time.start();
    while(time.elapsed() < 300)
        QCoreApplication::processEvents();
        tcpsocket->write(str.toUtf8().data());
        timer1.stop();

}

void ServerWidget::aiGetSocer()
{
    int personNum,aiNum,emptyNum;
    for (int i = 0; i < 20; i ++)
    {
        for (int j = 0; j < 20; j ++)
        {
            source[i][j]=0;
            if(chess[i][j] == 0)
            {
                chess[i][j] = 4 + peofirst;
                if (check_ok(i,j) == 1||(peofirst == 0&&check_ok(i,j)!=0))
                    continue;
                chess[i][j] = 0;
                for (int k = 0; k < 4; k ++)
                {
                    for (int p = -5; p <= 0; p ++)
                    {
                        personNum = 0;
                        aiNum = 0;
                        emptyNum = 0;
                        int dx = i + f[k][0] * p;
                        int dy = j + f[k][1] * p;
                        if (dx<0||dy<0||dx>=20||dy>=20)continue;
                        for (int l = 0; l < 6; l ++)
                        {
                            if (dx<0||dy<0||dx>=20||dy>=20)break;
                            if (chess[dx][dy] == 4 + peofirst)
                                aiNum ++;
                            else if (chess[dx][dy] == 0)
                                emptyNum ++;
                            else
                            {
                                if (l + p < 0)
                                    aiNum = 0,emptyNum = 0;
                                break;
                            }
                            //if (k==0)qDebug() << dx <<' '<< dy;
                            dx += f[k][0];
                            dy += f[k][1];
                        }
                        if (aiNum + emptyNum < 6) continue;
                        if (aiNum == 0)
                            source[i][j] += 5;
                        else if (aiNum == 1)
                            source[i][j] += 10;
                        else if (aiNum == 2)
                        {
                            source[i][j] += 20;
                        }
                        else if (aiNum == 3)
                        {
                            source[i][j] += 40;
                        }
                        else if (aiNum == 4)
                        {
                            source[i][j] += 200;
                        }
                        else if (aiNum == 5)
                        {
                            source[i][j] += 1000;
                        }

                    }
                }
                for (int k = 0; k < 4; k ++)
                {
                    for (int p = -5; p <= 0; p ++)
                    {
                        personNum = 0;
                        aiNum = 0;
                        emptyNum = 0;
                        int dx = i + f[k][0] * p;
                        int dy = j + f[k][1] * p;
                        if (dx<0||dy<0||dx>=20||dy>=20)continue;
                        for (int l = 0; l < 6; l ++)
                        {
                            if (dx<0||dy<0||dx>=20||dy>=20)break;
                            if (chess[dx][dy] == 4 + peofirst)
                            {
                                if (l + p < 0)
                                    personNum = 0,emptyNum = 0;
                                break;
                            }
                            else if (chess[dx][dy] == 0)
                                emptyNum ++;
                            else
                                personNum ++;
                            //if (k==0)qDebug() << dx <<' '<< dy;
                            dx += f[k][0];
                            dy += f[k][1];
                        }
                        if (personNum == 1)
                            source[i][j] += 10;
                        else if (personNum == 2)
                        {
                            source[i][j] += 30;
                        }
                        else if (personNum == 3)
                        {
                            source[i][j] += 60;
                        }
                        else if (personNum == 4)
                            source[i][j] += 300;
                        else if (personNum == 5)
                        {
                            source[i][j] += 1500;
                        }
                    }
                }
            }
        }
    }
}


void ServerWidget::search()
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

int ServerWidget::check_ok(int x,int y)
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
    if (chess[x][y] == 4&&temp5 >= 2)
    {
        return 2;
    }
    if (chess[x][y] == 4&&temp4 >= 2)
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

bool ServerWidget::check_victom(int x, int y)
{
    for (int i = 0; i < 4; i ++)
    if (chess_number[x][y][i] >= 6)
    {
        return 1;
    }

    return 0;
}

void ServerWidget::init()
{
    alreadydone=0;isStart=true;
    for (int i = 0; i < 20; i ++)
    {
        for (int j = 0; j < 20; j ++)
        {
            chess[i][j]=0;
            source[i][j]=0;
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

void ServerWidget::paintEvent(QPaintEvent *event)
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

ServerWidget::~ServerWidget()
{
    delete ui;
}
