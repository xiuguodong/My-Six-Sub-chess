using namespace std;
#include <iostream>
#define hang for (int i=1;i<=20;i++)
#define lie for (int j=1;j<=20;j++)
int map[25][25]; 
int mark[25][25][2];
int dir [][2]={{1,1},{1,-1},{0,1},{1,0}};
bool out (int x,int y)
{
	return x>=1&&x<=20&&y>=1&&y<=20;
}
int find (int zi,int f,int x,int y)
{
	int xx=x+dir[f][0];
	int yy=y+dir[f][1];
	if (!out(xx,yy)||map[xx][yy]!=zi) return 1;
	else {
		int x=1+find(zi,f,xx,yy);
		//cout<<x<<'\n';
		return x;
	}
	
}
int check()
{
	int flag=0;
	hang
	{
		lie 
		{
		    int flag1=0,flag2=0;
			if (map[i][j]==1)
			{
				flag1=max(flag1,find(1,0,i,j));
				flag1=max(flag1,find(1,1,i,j));
				flag1=max(flag1,find(1,2,i,j));
				flag1=max(flag1,find(1,3,i,j));
				if (flag1==6) flag=1;
				mark[i][j][0]=flag1;
			}
			if (map[i][j]==-1)
			{
				flag2=max(flag2,find(-1,0,i,j));
				flag2=max(flag2,find(-1,1,i,j));
				flag2=max(flag2,find(-1,2,i,j));
				flag2=max(flag2,find(-1,3,i,j));
				if (flag2==6) flag=-1;
				mark[i][j][1]=flag2;
			}
		}
	}
	return flag;
}
signed main (void)
{
	//ÆåÅÌ¶¨Òå£ºmap[1~20][1~20]£»
	//Æå×Ó¶¨Òå£º1->ºÚÆå£»-1->°×Æå£»0->¿Õ°×
	hang{
	    lie{
		    cin>>map[i][j];
		}
	}
	int flag=check();//0->Î´·ÖÊ¤¸º£»1->ºÚÆåÊ¤£»-1->°×ÆåÊ¤
	cout<<'\n'<<flag<<'\n';
	hang{
		lie{
			cout<<mark[i][j][0]<<' ';
		}
		cout<<'\n';
	}
	cout<<'\n';
	hang{
		lie{
		    cout<<mark[i][j][1]<<' ';
		}
		cout<<'\n';
	}
}
/*
1  0 -1 -1 1  0  1 1 0 0 1 1 1 1 1 -1 1 -1 0 0 
0  0 -1  1 1 -1  1 1 0 1 -1 -1 1 -1 0 0 -1 0 0 0
1 -1 -1  1 0  1 -1 1 1 0 0 0 0 0 0 0 0 0 0 0
-1 1 -1 -1 1  1 -1 1 1 0 0 0 0 0 0 0 0 0 0 0
0  0 -1  1 -1 1 -1 1 0 1 0 0 0 0 0 0 0 0 0 0
1 -1  1  1 1  1 -1 0 0 0 1 0 0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 
https://github.com/xiuguodong/My-Six-Sub-chess.git
*/ 
