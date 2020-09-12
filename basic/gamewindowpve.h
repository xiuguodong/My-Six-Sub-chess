#ifndef GAMEWINDOWPVE_H
#define GAMEWINDOWPVE_H

#include <QMainWindow>

namespace Ui {
class GameWindowPVE;
}

class GameWindowPVE : public QMainWindow
{
    Q_OBJECT

public:
    explicit GameWindowPVE(QWidget *parent = 0);
    ~GameWindowPVE();
    void init();
    int check_ok(int x,int y);//0->合法；1->该点超过边界；2->五五禁手；3->四四禁手
    bool check_victom(int x,int y);//0->未赢；1->胜利
    void search ();
    void actionByAi ();
    void aiGetSocer ();

private:
    Ui::GameWindowPVE *ui;
    void paintEvent(QPaintEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    int nextchess,alreadydone;
    int chess[20][20];int f[8][2];
    int chess_number[20][20][4];
    int movex,movey,currentx=-100,currenty=-100;
    int peofirst,waitforAi;
    int source[20][20];
};

#endif // GAMEWINDOWPVE_H
