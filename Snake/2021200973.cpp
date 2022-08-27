#include <bits/stdc++.h>
using namespace std;
#define _for(i, a, b) for (int i = (a); i < (b); ++i)
#define MAXX 100000
#define DEBUG_MODE
int action[2][4] = {{0, -1, 0, 1}, {-1, 0, 1, 0}}; // left up right down
int vis[40][40];                                   //标记障碍物
int vmap[40][40];                                  //标记价值
int Visit[40][40];                                 //标记搜索过
int mynum = 2021200973;
int areaValue[48];  // 48个区域分别搜索价值
#define alpha 0.7   //权重参数
#define beta 0.3    //
#define episode 0.1 //随机化参数
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
    int time;
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
        return this->val < p.val;
    }
    path()
    {
        val = 0;
        duration = 0;
    }
    path &operator=(const path &p)
    {
        this->x = p.x;
        this->y = p.y;
        this->dir = p.dir;
        this->duration = p.duration;
        this->val = p.val;
        return *this;
    }
};

void val_cal(game &g, path &p) // calculate the path val_cal
{

    int val_1 = 0; //道具收益
    int val_2 = 0; //区域收益
    _for(i, 0, p.x.size())
    {
        val_1 += vmap[p.x[i]][p.y[i]] * (10 - i); //递减
        val_2 += areaValue[p.x[i] / 5 * 8 + p.y[i] / 5];
        if (vmap[p.x[i]][p.y[i]] == 0) //价值相同优先走最快路径
        {
            val_1 -= 0.5; //路径惩罚
        }
    }

    p.val = val_1 * alpha + val_2 * beta;
}

game::game()
{
    int t;
    int k, b, n;
    cin >> t;
    time = t;
    cin >> k;
    re.clear();
    _for(i, 0, k)
    {
        reward p;
        cin >> p.x >> p.y >> p.v;
        this->re.push_back(p);
    }
    obs.clear();
    cin >> b;
    _for(i, 0, b)
    {
        obstacle p;
        cin >> p.x >> p.y >> p.r >> p.t;
        this->obs.push_back(p);
    }
    cin >> n;
    this->players.clear();
    _for(i, 0, n)
    {
        Snake p;
        cin >> p.number >> p.length >> p.score >> p.direction >> p.protection >> p.duration;
        _for(j, 0, p.length)
        {
            int x, y;
            cin >> x >> y;
            p.x.push_back(x);
            p.y.push_back(y);
        }
        this->players.push_back(p);
    }

    // mark other players
    _for(i, 0, this->players.size())
    {
        if (players[i].number == mynum)
            continue;
        _for(j, 0, this->players[i].length)
        {
            vis[this->players[i].x[j]][this->players[i].y[j]] = 1;
        }
        //标记蛇头周围的一格
        _for(k, 0, 2)
        {
            int tx = players[i].x[0] + action[0][i];
            int ty = players[i].y[0] + action[1][i];
            if (tx >= 0 && tx < 30 && ty >= 0 && ty < 40)
            {
                vis[tx][ty] = 1;
            }
        }
    }

    // mark the obstacle
    _for(i, 0, this->obs.size())
    {
        int r = this->obs[i].r; // effection
        _for(j, -r, r + 1)
        {
            _for(k, -r, r + 1)
            {
                if (abs(j) + abs(k) <= r)
                    vis[this->obs[i].x + j][this->obs[i].y + k] = 1;
            }
        }
    }

    // mark the pointvalue

    _for(i, 0, this->re.size())
    {
        if (re[i].v == -1)
            vmap[re[i].x][re[i].y] += 1; //增长道具
        else if (re[i].v == -2)
            vmap[re[i].x][re[i].y] += 2; //盾牌
        else
        {
            vmap[re[i].x][re[i].y] += re[i].v;
        }
    }

    // mark the area-value
    _for(i, 0, 30)
    {
        _for(j, 0, 40)
        {
            areaValue[i / 5 * 8 + j / 5] += vmap[i][j];
            areaValue[i / 5 * 8 + j / 5] -=vis[i][j];
        }
    }
}

int Mdis(int x1, int y1, int x2, int y2) // calculate the distance
{
    return abs(x1 - x2) + abs(y1 - y2);
};

Snake &search_snake(game &g, int number)
{
    _for(i, 0, g.players.size())
    {
        if (number == g.players[i].number)
            return g.players[i];
    }
    return g.players[0];
}

void dfs(game &g, int depth, path p, priority_queue<path> &q, int posx, int posy, int dir) //默认搜索10格以内的物品
{

    if (depth > 6)
    {
        val_cal(g, p);
        q.push(p);
        return;
    }
    else
    {
        int tx, ty;
        _for(i, 0, 4)
        {
            tx = posx + action[0][i];
            ty = posy + action[1][i];
            if (tx < 30 && tx >= 0 && ty >= 0 && ty < 40 && (Visit[tx][ty] <= 2) && (!vis[tx][ty])) // Do not run the wall
            {
                Visit[tx][ty]++;
                if (dir - i != 2 && i - dir != 2)
                {
                    p.dir.push_back(i); //存入方向
                    p.x.push_back(tx);
                    p.y.push_back(ty);
                    dfs(g, depth + 1, p, q, tx, ty, i);
                    p.dir.pop_back();
                    p.x.pop_back();
                    p.y.pop_back();
                }
                Visit[tx][ty]--;
            }
        }
    }
    return;
}

void play()
{

    game g;
    Snake me = search_snake(g, mynum); // get the infomation about myself
    priority_queue<path> q;
    path p;
    p.duration = me.duration;
    dfs(g, 0, p, q, me.x[0], me.y[0], me.direction); // get all the path
    path ans;
    ans = q.top();
    if (ans.val < 0 && me.protection > 0)
        cout << '4';
    else
    {
        cout << ans.dir[0];
    }

    // debug
    //  cout << endl
    //       << ans.val << endl
    //       << ans.x[0] << " " << ans.y[0];
}
int main()
{
    play();

    // system("pause");
    return 0;
}
/*
152
100
6 0 5
20 0 2
13 0 2
13 1 1
5 1 2
0 1 1
28 2 -1
13 2 2
9 3 2
9 3 2
17 3 2
15 4 1
0 4 1
5 5 2
3 5 -1
10 5 5
7 6 3
14 6 3
12 7 5
3 7 5
25 7 1
26 8 5
26 8 3
19 9 3
11 9 1
20 9 1
10 10 1
6 10 1
3 11 1
29 11 -1
4 11 3
20 12 1
28 12 -1
14 12 3
27 13 3
24 13 5
0 14 3
27 14 2
5 14 5
3 15 -1
25 15 5
12 16 2
27 16 -1
17 16 -1
27 17 2
4 17 3
24 18 3
15 18 2
29 18 2
21 19 3
20 19 1
11 20 5
22 20 1
3 20 1
9 21 2
2 21 1
5 22 1
17 22 3
6 22 -1
18 23 1
3 23 1
2 23 1
7 24 1
28 24 2
3 25 1
26 25 -1
19 25 3
25 26 1
10 26 2
18 27 1
22 27 1
27 27 3
15 28 2
21 28 5
27 29 1
21 29 1
14 29 1
2 30 1
18 30 5
16 31 2
3 31 1
18 31 3
25 32 5
12 32 2
6 33 5
7 33 1
10 33 1
12 34 3
16 34 1
1 34 1
18 35 1
12 35 2
11 36 3
17 36 1
25 36 1
0 37 -1
13 28 1
2 38 1
28 28 -2
12 38 1
0
1
2021200973 15 22 3 2 0
29 39
28 39
27 39
26 39
25 39
24 39
23 39
22 39
22 38
21 38
20 38
19 38
18 38
17 38
16 38
*/