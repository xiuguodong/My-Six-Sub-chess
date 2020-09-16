#ifndef CLIENTWIDGET_H
#define CLIENTWIDGET_H

#include <QWidget>
#include <QMainWindow>
#include <QPainter>
#include <QTcpSocket>
#include <QTimer>

namespace Ui {
class ClientWidget;
}

class ClientWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ClientWidget(QWidget *parent = 0);
    ~ClientWidget();
    void init();
    int check_ok(int x,int y);//0->合法；1->该点超过边界；2->五五禁手；3->四四禁手
    bool check_victom(int x,int y);//0->未赢；1->胜利
    void search ();

private:
    Ui::ClientWidget *ui;

    QTcpSocket *tcpsocket;
    void paintEvent(QPaintEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    int nextchess,alreadydone;
    int chess[20][20];int f[8][2];
    int chess_number[20][20][4];
    int movex,movey,currentx=-100,currenty=-100;
    int peofirst,startai,mark;
    QTimer timer;
};

#endif // CLIENTWIDGET_H
