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
        int temp = 0;
        for (int i = 0; i < 4; i ++)
        {
            if (chess_number[x][y][i] == 5)
            {
                int number = 0;
                for (int j = 1; j < 6; j ++)
                {
                    int dx = x + f[i][0] * j;
                    int dy = y + f[i][1] * j;
                    if (dx<0||dy<0||dx>=20||dy>=20||chess[dx][dy]==5)
                        break;
                    if (chess[dx][dy]==0) number ++;
                }
                for (int j = 1; j < 6; j ++)
                {
                    int dx = x + f[i+4][0] * j;
                    int dy = y + f[i+4][1] * j;
                    if (dx<0||dy<0||dx>=20||dy>=20||chess[dx][dy]==5)
                        break;
                    if (chess[dx][dy]==0) number ++;
                }
                if (number>0) temp ++;
            }
        }
        if (temp >= 2)
            return 2;
    }
    if (nextchess == 0&&temp4 >= 2)
    {
        int temp = 0;
        for (int i = 0; i < 4; i ++)
        {
            if (chess_number[x][y][i] == 4)
            {
                int number1 = 0,number2 = 0,j;//1->不连续点数目，2->两侧空出点数目
                //1的值大于1不禁手，等于1但2的值小于1不禁手，等于0但2的值小于2不禁手
                for (j = 1; j < 6; j ++)
                {
                    int dx = x + f[i][0] * j;
                    int dy = y + f[i][1] * j;
                    if (dx<0||dy<0||dx>=20||dy>=20||chess[dx][dy]==5)
                        break;
                }
                for (j --; j > 0; j --)
                {
                    int dx = x + f[i][0] * j;
                    int dy = y + f[i][1] * j;
                    if (chess[dx][dy] == 0) number2++;
                    if (chess[dx][dy] == 4) break;
                }
                if (number2 == 0) continue;
                for (j --; j > 0; j --)
                {
                    int dx = x + f[i][0] * j;
                    int dy = y + f[i][1] * j;
                    if (chess[dx][dy] == 0) number1++;
                }

                for (j = 1; j < 6; j ++)
                {
                    int dx = x + f[i+4][0] * j;
                    int dy = y + f[i+4][1] * j;
                    if (dx<0||dy<0||dx>=20||dy>=20||chess[dx][dy]==5)
                        break;
                }
                int number = 0;
                for (j --; j > 0; j --)
                {
                    int dx = x + f[i+4][0] * j;
                    int dy = y + f[i+4][1] * j;
                    if (chess[dx][dy] == 0) number2++,number++;
                    if (chess[dx][dy] == 4) break;
                }
                if (number == 0) continue;
                for (j --; j > 0; j --)
                {
                    int dx = x + f[i+4][0] * j;
                    int dy = y + f[i+4][1] * j;
                    if (chess[dx][dy] == 0) number1++;
                }
                qDebug()<<number1<<' '<<number2;
                if (number1>1||(number1==1&&number2<1)||(number1==0&&number2<2))
                {
                    continue;
                    qDebug()<<temp;
                }
                temp++;
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
        int number = 1;
        for (int j = 1; j < 6; j ++, number ++)
        {
            int dx = x + f[i][0] * j;
            int dy = y + f[i][1] * j;
            if (dx<0||dy<0||dx>=20||dy>=20||chess[dx][dy]==5-nextchess||chess[dx][dy]==0)
                break;
        }
        for (int j = 1; j < 6; j ++, number ++)
        {
            int dx = x + f[i+4][0] * j;
            int dy = y + f[i+4][1] * j;
            if (dx<0||dy<0||dx>=20||dy>=20||chess[dx][dy]==5-nextchess||chess[dx][dy]==0)
                break;
        }
        if (number >= 6)
            return 1;
    }

    return 0;
}
