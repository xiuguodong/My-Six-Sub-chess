#include "checkfunction.h"
#include "gamewindow.h"
#include "startfunctiong.h"
#include <QDebug>

CheckFunction::CheckFunction()
{

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
