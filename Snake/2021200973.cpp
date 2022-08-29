#include <bits/stdc++.h>
using namespace std;
#define _for(i, a, b) for (int i = (a); i < (b); ++i)
#define MAXX 100000
#define DEBUG_MODE
int action[2][8] = {{0, -1, 0, 1, 0, -2, 0, 2}, {-1, 0, 1, 0, -2, 0, 2, 0}}; // left up right down
int vis[40][40];                                                             //标记障碍物
int vmap[40][40];                                                            //标记价值
int Visit[40][40];                                                           //标记搜索过
int mynum = 2021200973;
int areaValue[48]; // 48个区域分别搜索价值
#define alpha 0.7  //资源参数
#define beta 0.4   //区域参数
#define episode 0  //中心参数
#define maxdepth 10
#define gamma 1 //路径惩罚参数
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
int Mdis(int x1, int y1, int x2, int y2);
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
            val_1 -= gamma; //路径惩罚
        }
    }

    p.val = val_1 * alpha + val_2 * beta;
}

int eva(int i, int j) //计算地理位置加成
{
    int d = Mdis(i, j, 15, 20); //曼哈顿距离
    d = -1 * d * d * episode;   //平方
    return d;
}
game::game()
{

    int flag;
    int t;
    int k, b, n;
    cin >> t;
    time = t;
    cin >> k;
    int pval = 2;
    if (n > 4 && time > 50 && time < 200)
        pval = 3;
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
        if (players[i].number != mynum)

        {
            _for(j, 0, this->players[i].length)
            {
                vis[this->players[i].x[j]][this->players[i].y[j]] = 1;
            }
            //标记蛇头周围的2格
            vis[players[i].x[0]][players[i].y[0]]=players[i].score;
            if(players[i].score>30){
            _for(k, 0, 8)
            {

                int tx = players[i].x[0] + action[0][k];
                int ty = players[i].y[0] + action[1][k];
                if (tx >= 0 && tx < 30 && ty >= 0 && ty < 40)
                {
                    vis[tx][ty] = players[i].score;
                }
            }
            }
            else
            {
                _for(k, 0, 4)
            {

                int tx = players[i].x[0] + action[0][k];
                int ty = players[i].y[0] + action[1][k];
                if (tx >= 0 && tx < 30 && ty >= 0 && ty < 40)
                {
                    vis[tx][ty] = players[i].score;
                }
            }
            }
        }
        else
        {
            flag = i;
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
    int lval = 1;
    if (players[flag].protection < 5)
        pval = 5;
    if (time > 200)
        lval = 2;
    _for(i, 0, this->re.size())
    {
        if (re[i].v == -1)
            vmap[re[i].x][re[i].y] += lval; //增长道具
        else if (re[i].v == -2)
            vmap[re[i].x][re[i].y] += pval; //盾牌
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
            areaValue[i / 5 * 8 + j / 5] += vmap[i][j]; //资源密集加成
            areaValue[i / 5 * 8 + j / 5] -= vis[i][j];  //障碍惩罚
            areaValue[i / 5 * 8 + j / 5] += eva(i, j);  //中心加成
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

    if (depth >= maxdepth)
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
            if (tx < 30 && tx >= 0 && ty >= 0 && ty < 40 && Visit[tx][ty] <= 2 && (p.duration > 1 || vis[tx][ty]==0)) // Do not run the wall
            {
                Visit[tx][ty]++;
                if (dir - i != 2 && i - dir != 2)
                {
                    p.dir.push_back(i); //存入方向
                    p.x.push_back(tx);
                    p.y.push_back(ty);
                    p.duration--;
                    dfs(g, depth + 1, p, q, tx, ty, i);
                    p.duration++;
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

bool is_protect(Snake &me, game &g, path &ans) //判断是否开盾
{

    int tx = ans.x[0];
    int ty = ans.y[0];
    if (vis[tx][ty] > 2 && me.duration < 4)
    {
        return true;
    }

    if (g.time < 30 && me.duration < 2)
    {

        return true;
    }
    return false;
}
bool is_attack(Snake &me, path &ans)
{
    int tx = ans.x[0];
    int ty = ans.y[0];
    if (vis[tx][ty] > 50&&me.score>30)
    {
        cout<<"5 "<<tx-me.x[0]<<" "<<ty-me.y[0];
        return true;
    }
    return false;
}

void play()
{

    game g;
    Snake me = search_snake(g, mynum); // get the infomation about myself
    priority_queue<path> q;
    path p;
    p.duration = me.duration-1;
    dfs(g, 0, p, q, me.x[0], me.y[0], me.direction); // get all the path
    path ans;
    ans = q.top();
    if (is_protect(me, g, ans))
        cout << '4';
    else if(is_attack(me,ans));
    else 
    {
        cout << ans.dir[0];
    }
}
int main()
{
    play();

    // system("pause");
    return 0;
}