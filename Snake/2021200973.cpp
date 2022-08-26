#include <bits/stdc++.h>
using namespace std;
#define _for(i, m, n) for(int i = (m); i <(n); ++i)
#define MINN -100000
int action[2][4] = {{0,0,1,-1},{1,-1,0,0}};
// right left down up
struct reward
{
    int x, y, v;
};

struct obstacle
{
    int x, y, r, t;
};

class Snake
{
public:
    int number;    //学号
    int length;    //长度
    int score;     //得分
    int direction; //方向
    // 0：左  1：上  2：右  3：下  4:盾牌
    int protection; //盾牌数量
    int duration;   //当前盾牌持续时间
    vector<int> x;
    vector<int> y;
};

class game
{
public:
    vector<reward> re;
    vector<obstacle> obs;
    vector<Snake> players;
    game();
};

class path
{
public:
    int val; // the value
    vector<int> x;
    vector<int> y;
    vector<int> dir; // the direction
    int duration;    // the states of the protection
    bool operator<(const path &p) const
    {
        return this->val > p.val;
    }
    path()
    {
        val=0;
        duration=0;
    }
};

bool is_possible_path(game &G, int duration, path &p) //没有判断是否会掉出地图
{
    int vis[40][40];
    memset(vis, 0, sizeof(vis));
    if (duration > 1) //有保护状态
    {
        return true;
    }
    else
    {
        // mark other players
        _for (i, 0, G.players.size())
        {
            _for (j, 0, G.players[i].length)
            {
                vis[G.players[i].x[j]][G.players[i].y[j]] = 1;
            }
        }

        // mark the obstacle
        _for (i, 0, G.obs.size())
        {
            int r = G.obs[i].r; // effection
            _for (j, 0, r + 1)
            {
                _for (k, 0, r + 1)
                {
                    if (j + k <= r)
                        vis[G.obs[i].x + j][G.obs[i].y + k] = 1;
                }
            }
        }

        // check the path
        _for (i, 0, p.x.size())
        {
            if (vis[p.x[i]][p.y[i]])
                return false;
        }
    }
    return true;
}
void val_cal(game &g, path &p) // calculate the path val_cal
{
    int vmap[40][40];
    int val = 0;
    memset(vmap, 0, sizeof(vmap));
    _for (i, 0, g.re.size())
    {
        vmap[g.re[i].x][g.re[i].y] += g.re[i].v;
    }
    _for (i, 0, p.x.size())
    {

        val += vmap[p.x[i]][p.y[i]];
    }
    if (!is_possible_path(g, p.duration, p))
    {
        val -= MINN;
    }
    
    p.val = val;
}

game::game()
{
    int t;
    int k, b, n;
    cin >> t;
    cin >> k;
    re.clear();
    _for (i, 0, k)
    {
        reward p;
        cin >> p.x >> p.y >> p.v;
        this->re.push_back(p);
    }
    obs.clear();
    cin >> b;
    _for (i, 0, b)
    {
        obstacle p;
        cin >> p.x >> p.y >> p.r >> p.t;
        this->obs.push_back(p);
    }
    cin >> n;
    this->players.clear();
    _for (i, 0, n)
    {
        Snake p;
        cin >> p.number >> p.length >>p.score >> p.direction >> p.protection >> p.duration;
        _for (j, 0, p.length)
        {
            int x, y;
            cin >> x >> y;
            p.x.push_back(x);
            p.y.push_back(y);
        }
        this->players.push_back(p);
    }
}

int Mdis(int x1, int y1, int x2, int y2) // calculate the distance
{
    return abs(x1 - x2) + abs(y1 - y2);
};

Snake &search_snake(game &g, int number)
{
    _for (i, 0, g.players.size())
    {
        if (number == g.players[i].number)
            return g.players[i];
    }
    
}


int Visit[40][40];
void dfs(game &g,int depth, path p, priority_queue<path> &q, int posx, int posy) //默认搜索10格以内的物品
{
    p.x.push_back(posx);
    p.y.push_back(posy);
    if (depth > 10)
    {
        val_cal(g, p);
        q.push(p);
        return;
    }
    else
    {
        int tx, ty;

        _for (i, 0, 4)
        {
            tx = posx + action[0][i];
            ty = posy + action[1][i];
            if (tx < 40 && tx >= 0 && ty >= 0 && ty < 40&&(!Visit[tx][ty])) // Do not run the wall
            {
                Visit[tx][ty]=1;
                if (tx > posx)
                    p.dir.push_back(2);
                else if (tx < posx)
                    p.dir.push_back(0);
                else if (ty > posy)
                    p.dir.push_back(3);
                else if (ty < posy)
                    p.dir.push_back(1);
                dfs(g, depth + 1, p, q, tx, ty);
                Visit[tx][ty]=0;
            }
        }
    }
    return;
}
int mynum=2021200973;
void play()
{

    game g;
    Snake me = search_snake(g, mynum); // get the infomation about myself
    vector<path> ans;
    priority_queue<path> q;
    path p;
    dfs(g, 0, p, q, me.x[0], me.y[0]); // get all the path
    p = q.top();
    if (p.val < 0 && me.protection > 0)
        cout << "4";
    cout << p.dir[0];
}
int main()
{
    play();
    
    return 0;
}