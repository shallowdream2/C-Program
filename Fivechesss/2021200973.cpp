#include<bits/stdc++.h>
using namespace std;

class chess
{
    public:
    int player;//当前执黑还是执白
    int chess[16][16];//棋盘
    chess();
    int check();//判定当前棋盘状态
    int vis[15][15];
};
chess::chess{
    cin>>player;
    for(int i=0;i<15;++i)
    {
        for(int j=0;j<15;++j)
        {
            cin>>chess[i][j];
        }
    }
    memset(vis,0,sizeof(vis));
    for(int i=0;i<15;++i)
    {
        for(int j=0;j<15;++j)
        {
            if(chess[i][j]==0)
            {
                for(int k=-2;k<3;++k)
                {
                    for(int l=-2;l<3;++l)
                    {
                        if(i+k>=0&&i+k<15&&j+l>=0&&j+l<15)
                        {
                            vis[i+k][j+l]=1;
                        }
                    }
                }
            }
        }
    }
    
};

int chess::check()
{
    for(int i=0;i<15;++i)
    {
        for(int j=0;j<15;++j)
        {
            if(vis[i][j]==0)return 1;
        }
    }
    return 0;
    
};

int evaluate(chess &Chess)
{
    
}





int main()
{

}