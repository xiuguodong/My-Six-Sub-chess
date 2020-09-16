#include "clientwidget.h"
#include "ui_clientwidget.h"
#include <QHostAddress>
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

ClientWidget::ClientWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ClientWidget)
{
    tcpsocket =NULL;
    tcpsocket = new QTcpSocket(this);
    init();
    ui->setupUi(this);
    this->setFixedSize(660,680);
    this->setAutoFillBackground(true);
    QPalette palette;
    palette.setColor(QPalette::Background,QColor("#B1723C"));
    this->setPalette(palette);//设置背景色！
    //centralWidget()->setMouseTracking(true);
    setMouseTracking(true);
    setWindowTitle("客户端");
    connect(tcpsocket,&QTcpSocket::connected,
            [=]()
    {
        qDebug() << "成功和服务器建立好连接";
    }
    );
    //收消息
    mark = 0;
    connect(tcpsocket,&QTcpSocket::readyRead,
            [=]()
    {
        //从通信套接字中取出内容
        QByteArray array =  tcpsocket->readAll();
        qDebug()<<array;
        if (mark == 0)
        {
            int flag=QString(array).section("##",0,0).toInt();
            alreadydone=QString(array).section("##",1,1).toInt();
            currentx=QString(array).section("##",2,2).toInt();
            currenty=QString(array).section("##",3,3).toInt();
            qDebug()<<flag<<' '<<alreadydone<<' '<<currentx<<' '<<currenty;
            update();
            if (flag == 1)
            {
                QMessageBox::about(this,"end","黑棋胜利");
                tcpsocket->disconnectFromHost();
                this->close();
            }
            if (flag == -1)
            {
                QMessageBox::about(this,"end","白棋胜利");
                tcpsocket->disconnectFromHost();
                this->close();
            }
            if (alreadydone == 400)
            {
                QMessageBox::about(this,"end","黑棋胜利");
                tcpsocket->disconnectFromHost();
                this->close();
            }
            mark = 1;
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
            peofirst = 1;
            update();
            mark = 0;
        }
    }

    );
}

void ClientWidget::mouseMoveEvent(QMouseEvent *event)//!!!!
{
    movex = (event->y() - 45 )/ 30;
    movey = (event->x() - 25 )/ 30;
    //qDebug() << event->x() << ' ' <<event->y();
    QWidget::update();
}



void ClientWidget::mousePressEvent(QMouseEvent *event)
{
    if (peofirst == 0)
        return ;
    if (nextchess==0)
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
        alreadydone++;
        QWidget::update();
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
        alreadydone++;
        QWidget::update();
    }
    QTime time;
    time.start();
    while(time.elapsed() < 300)
        QCoreApplication::processEvents();
        tcpsocket->write((char*)chess,sizeof chess);
        timer.stop();
    peofirst = 0;
}


void ClientWidget::init()
{
    nextchess=0,alreadydone=0;mark=0;
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
    int ret = QMessageBox::question(this,"对抗方式","是否人机",
                          QMessageBox::Yes,QMessageBox::No);
    switch (ret)
    {
        case QMessageBox::Yes:
            {
                int ret1 = QMessageBox::question(this,"人机对抗","是否先走？",
                                      QMessageBox::Yes,QMessageBox::No);
                switch (ret1)
                {
                    case QMessageBox::Yes:
                        peofirst = 1;
                        nextchess = 0;
                        startai = 1;
                        break;
                    case QMessageBox::No:
                        peofirst = 0;
                        nextchess = 1;
                        startai = 1;
                        break;
                }
                break;
            }
        case QMessageBox::No:
            {
                int ret2 = QMessageBox::question(this,"人人对抗","是否先走？",
                                      QMessageBox::Yes,QMessageBox::No);
                switch (ret2)
                {
                    case QMessageBox::Yes:
                        peofirst = 1;
                        nextchess = 0;
                        startai = 0;
                        break;
                    case QMessageBox::No:
                        peofirst = 0;
                        nextchess = 1;
                        startai = 0;
                        break;
                    default:
                        break;
                }
            }
    }
    QString strip = "192.168.43.153";
    qint16 prot = 8888;
    //主动和服务器建立连接
    tcpsocket->connectToHost(QHostAddress(strip),prot);
    QString str = QString ("%1##%2")
            .arg(peofirst).arg(startai);
    tcpsocket->write(str.toUtf8());

    if (peofirst == 0)
    {
        tcpsocket->write((char*)chess,sizeof chess);
    }
}

void ClientWidget::paintEvent(QPaintEvent *event)
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


ClientWidget::~ClientWidget()
{
    delete ui;
}
