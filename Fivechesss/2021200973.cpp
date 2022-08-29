#include <bits/stdc++.h>
using namespace std;
//定义不同局面价值
#define live_Five = 1000000
#define live_Four = 100000
#define sleep_Four = 10000
#define live_Three = 1000
#define live_Two = 100
#define leep_Three = 100
#define live_One = 10
#define sleep_Two = 10
#define sleep_One = 1
#define un_known = 0
//五元组计算规则
int eva(vector<int>a)//假设自己是黑棋
{
    int flag1, flag2, flag3;
    flag1 = flag2 = flag3 = 0;
    for (int i = 0; i < 5; ++i)
    {
        if (a[i] == 0)
            flag1++;
        if (a[i] == 1)
            flag2++;
        if (a[i] == -1)
            flag3++;
    }
    if(flag1>0&&flag2>0)return 0;
    if(flag1==1&&flag3==4)return 1;
    if(flag1==2&&flag3==3)return 10;
    if(flag1==3&&flag3==2)return 100;
    if(flag1==4&&flag3==1)return 10000;
    if(flag1==5)return 1000000;
    if(flag2==1&&flag3==4)return -1;
    if(flag2==2&&flag3==3)return -10;
    if(a[0]==-1&&a[4]==-1&&flag2==3)return -2000;
    if(flag2==3&&flag3==2)return -1000;
    if(flag2==4&&flag3==1)return -10000;
    if(flag2==5)return -1000000;
}

class chess
{
public:
    int player;        //当前执黑还是执白
    int board[16][16]; //棋盘
    chess();
    int check(); //判定当前棋盘状态
    int vis[15][15];
    int evaluate();
};

chess::chess
{
    cin >> player;
    for (int i = 0; i < 15; ++i)
    {
        for (int j = 0; j < 15; ++j)
        {
            cin >> board[i][j];
        }
    }
    memset(vis, 0, sizeof(vis));
    for (int i = 0; i < 15; ++i)
    {
        for (int j = 0; j < 15; ++j)
        {
            int temp[5];
                for (int k = -2; k < 3; ++k)
                {
                    for (int l = -2; l < 3; ++l)
                    {
                        if (i + k >= 0 && i + k < 15 && j + l >= 0 && j + l < 15)
                        {
                            if ((abs(l) + abs(k) <= 5) || (k - l == 0) || (k + l == 0))
                            {
                                vis[i + k][j + l] = 1;
                            }
                        }
                    }
                }
            
        }
    }
};

int chess::check()
{
    for (int i = 0; i < 15; ++i)
    {
        for (int j = 0; j < 15; ++j)
        {
            if (vis[i][j] == 0)
                return 1;
        }
    }
    return 0;
};

int chess::evaluate() //评估局面得分
{
    
    int score = 0;
    for (int i = 0; i < 15; ++i)
    {
        for (int j = 0; j < 15; ++j)
        {
          if(board[i][j]!=0)
          {
            vector<int>a;//行五元组
            vector<int>b;//列五元组
            vector<int>c;//正对角线
            vector<int>d;//副对角线

            for(int k=-2;k<=2;++k)
            {
                if(j+k>=0&&j+k<15)a.push_back(board[i][j+k]);
                if(i+k>=0&&i+k<15)b.push_back(board[i+k][j]);
                
            }
            if(a.size()==5)score+=eva(a);
            
            


          }
           
        }
    }
}

int AlphaBeta(int depth, int alpha, int beta, chess Chess)
{
    if (depth == 0)
    {
        return Chess.evaluate();
    }
    for (int i = 0; i < 15; ++i)
    {
        for (int j = 0; j < 15; ++j)
        {
        }
    }
}

int main()
{
}