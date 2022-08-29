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
#define maxDepth 5 //最大搜索深度
#define minn -10000000
#define maxx 10000000
#define area_r 3
#define DEBUGMODE 0
//五元组计算规则
int eva(vector<int> a, int player) //假设自己是黑棋
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
    if (player == 1)
    {
        int t = flag1;
        flag1 = flag2;
        flag2 = t;
    }
    if (flag1 > 0 && flag2 > 0)
        return 0;
    if (flag1 == 1 && flag3 == 4)
        return 1;
    if (flag1 == 2 && flag3 == 3)
        return 10;
    if (flag1 == 3 && flag3 == 2)
        return 100;
    if (flag1 == 4 && flag3 == 1)
        return 10000;
    if (flag1 == 5)
        return 1000000;
    if (flag2 == 1 && flag3 == 4)
        return -1;
    if (flag2 == 2 && flag3 == 3)
        return -10;
    if (a[0] == -1 && a[4] == -1 && flag2 == 3)
        return -2000;
    if (flag2 == 3 && flag3 == 2)
        return -1000;
    if (flag2 == 4 && flag3 == 1)
        return -10000;
    if (flag2 == 5)
        return -1000000;
    return 0;
};

class chess
{
public:
    int player;        //当前执黑还是执白
    int board[16][16]; //棋盘
    int depth;         //当前节点神单独
    chess *father;
    set<chess *> child;
    int X;
    int Y;
    int value;
    bool is_max_node();
    int iswin();
    chess();
    chess(chess &nowchess, int X, int Y);
    void evaluate();
};

chess::chess()
{
    father = nullptr;
    this->child.clear();
    value = INT32_MIN;
    depth = X = Y = 0;
    cin >> player;
    for (int i = 0; i < 15; ++i)
    {
        for (int j = 0; j < 15; ++j)
        {
            cin >> board[i][j];
        }
    }
};
chess::chess(chess &nowchess, int X, int Y)
{
    depth = nowchess.depth + 1;
    value = is_max_node() ? minn : maxx; //对不同节点初始化方式不同
    father = &nowchess;
    child.clear();
    this->X = X;
    this->Y = Y;
    memcpy(board, nowchess.board, sizeof(nowchess.board));
    if (player == 0)
        board[X][Y] = (depth & 1) ^ 1;
    else
        board[X][Y] = (depth % 1);
    player = nowchess.player;
}

void chess::evaluate() //评估局面得分
{

    int score = 0;
    vector<int> a; //行五元组
    vector<int> b; //列五元组
    vector<int> c; //正对角线
    vector<int> d; //副对角线
    for (int i = 0; i < 15; ++i)
    {
        for (int j = 0; j < 15; ++j)
        {
            a.clear();
            b.clear();
            c.clear();
            d.clear();
            for (int k = 0; k <= 4; ++k)
            {
                if (j + k >= 0 && j + k < 15)
                    a.push_back(board[i][j + k]);
                if (i + k >= 0 && i + k < 15)
                    b.push_back(board[i + k][j]);
                if (j + k >= 0 && j + k < 15 && i + k >= 0 && i + k < 15)
                {
                    c.push_back(board[i + k][j + k]); // \对角线
                }
                if (j - k >= 0 && j - k < 15 && i + k >= 0 && i + k < 15)
                {
                    d.push_back(board[i + k][j - k]); // /对角线
                }
            }
            if (a.size() == 5)
            {
                score += eva(a, 0) - eva(a, 1);
            }
            if (b.size() == 5)
            {
                score += eva(b, 0) - eva(b, 1);
            }

            if (c.size() == 5)
            {
                score += eva(c, 0) - eva(c, 1);
            }
            if (d.size() == 5)
            {
                score += eva(d, 0) - eva(d, 1);
            }
        }
    }
    value = score;
}

bool chess::is_max_node() // 0层为根节点
{
    return (depth & 1) ^ 1;
};

int chess::iswin()
{
    if (value >= 1000000 && value != maxx && value != minn)
    {
        return 1;
    }
    return 0;
}

class Tree
{
public:
    chess *root;
    chess *ans;
    deque<chess *> open;
    void choose_pos();
    int minimax(); //主函数
    // void dfs();
    int try_to(chess *ch);                              //返回可探索节点数
    vector<pair<int, int>> get_possible_pos(chess *ch); //待扩展位置
    bool is_cut(chess *ch);                             //判断能否剪枝
    void update(chess *ch);                             //更新
    Tree();
    ~Tree(); // default
};

Tree::Tree()
{
    root = new chess;
    ans = nullptr;
}
Tree::~Tree()
{
    delete root;
}
void Tree::choose_pos()
{
    ans = *root->child.begin();
    for (auto n = root->child.begin(); n != root->child.end(); ++n)
    {
        if ((*n)->value > ans->value)
            ans = *n;
        if (DEBUGMODE)
            cout << (*n)->X << " " << (*n)->Y << " " << (*n)->value << endl;
    }
}
vector<pair<int, int>> Tree::get_possible_pos(chess *ch)
{
    bool first = true;
    int vis[16][16];
    memset(vis, 0, sizeof(vis));
    vector<pair<int, int>> t;
    for (int i = 0; i < 15; ++i)
    {
        for (int j = 0; j < 15; ++j)
        {
            if (ch->board[i][j] == -1)
                continue;
            first = false;
            for (int k = -area_r; k <= area_r; ++k)
            {
                for (int l = -area_r; l <= area_r; ++l)
                {
                    if (i + k >= 0 && i + k < 15 && j + l >= 0 && j + l < 15 && ch->board[i + k][j + l] == -1 && !vis[i + k][j + l])
                        vis[i + k][j + l] = 1;
                }
            }
        }
    }
    if (first)
        t.push_back(make_pair(7, 7)); //天元开局
    for (int i = 0; i < 15; ++i)
    {
        for (int j = 0; j < 15; ++j)
        {
            if (vis[i][j])
            {
                t.push_back(make_pair(i, j));
            }
        }
    }
    return t;
}

int Tree::try_to(chess *ch) //新建子节点
{
    vector<pair<int, int>> goal = get_possible_pos(ch);
    for (int i = 0; i < goal.size(); ++i)
    {
        chess *temp = new chess(*ch, goal[i].first, goal[i].second);
        ch->child.insert(temp);
        open.push_front(temp);
    }
    return goal.size();
};

bool Tree::is_cut(chess *ch)
{
    if (ch == nullptr || ch->father == nullptr)
        return false;
    if (ch->is_max_node() && ch->value > ch->father->value)
        return true; // max节点剪枝规则
    if ((!ch->is_max_node()) && ch->value < ch->father->value)
        return true;
    return is_cut(ch->father);
}
void Tree::update(chess *ch)
{
    if (ch == nullptr)
        return;
    if (ch->child.empty())
    {
        update(ch->father);
        return;
    }
    if (ch->is_max_node()) // max节点
    {
        int val = minn;
        for (auto i = ch->child.begin(); i != ch->child.end(); ++i)
        {
            if ((*i)->value != maxx)
                val = max(val, (*i)->value); //所有子节点中估值最大的
        }
        if (val > ch->value)
        {
            ch->value = val;
            update(ch->father);
        }
    }
    else // min节点
    {
        int val = maxx;
        for (auto i = ch->child.begin(); i != ch->child.end(); i++)
        {
            if ((*i)->value != minn)
                val = min(val, (*i)->value); //所有子节点估值最小的
        }
        if (val < ch->value)
        {
            ch->value = val;
            update(ch->father);
        }
    }
}

int Tree::minimax()
{
    if (root->iswin() == 1)
        return 1;
    open.push_back(root);
    while (!open.empty())
    {
        chess *ch = open.front();
        open.pop_front();
        if (is_cut(ch->father))
            continue;
        if (ch->depth < maxDepth)
        {
            int num = try_to(ch);

            if (num != 0)
                continue;
        }
        ch->evaluate();
        update(ch);
    }
    choose_pos();
    return 0;
}

int main()
{
    Tree tr;
    tr.minimax();
    cout << tr.ans->X << " " << tr.ans->Y;
    // cout<<tr.ans->value;
     //system("pause");
}
