#ifndef SERVERWIDGET_H
#define SERVERWIDGET_H

#include <QWidget>
#include <QTcpServer>
#include <QTcpSocket>
#include <QTimer>

namespace Ui {
class ServerWidget;
}

class ServerWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ServerWidget(QWidget *parent = 0);
    ~ServerWidget();
    void search();
    void init();
    int check_ok(int x,int y);//0->合法；1->该点超过边界；2->五五禁手；3->四四禁手
    bool check_victom(int x,int y);//0->未赢；1->胜利
    void actionByAi ();
    void aiGetSocer ();

private:
    Ui::ServerWidget *ui;
    void paintEvent(QPaintEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    QTcpServer *tcpserver;
    QTcpSocket *tcpsocket;
    int chess[20][20]; //b->黑;w->白;s->空
    int chess_number [20][20][4];
    int f[8][2];
    int movex,movey,currentx=-100,currenty=-100;
    int alreadydone,flag;
    int peofirst,waitforAi,startai,waitforpeo;
    int source[20][20];
    bool isStart;
    QTimer timer;
    QTimer timer1;
};

#endif // SERVERWIDGET_H
