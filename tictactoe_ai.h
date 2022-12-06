#include <iostream>
#include <fstream>
#include <random>
#include <vector>
using namespace std;
#define vi vector<int>
#define vvi vector<vi>
#define piv pair<int,vi>
#define pid pair<int,double>
const int pow3[10] = {1,3,9,27,81,243,729,2187,6561,19683};
const vi rCW{2,5,8,1,4,7,0,3,6}, rLR{2,1,0,5,4,3,8,7,6};
const vvi lines{{0,1,2},{3,4,5},{6,7,8},{0,3,6},{1,4,7},{2,5,8},{0,4,8},{2,4,6}};
vi cmp(vi a, vi b)
{
    vi c(9);
    for (int i = 0; i < 9; i++) c[i] = b[a[i]];
    return c;
}
vi inv(vi a)
{
    vi b(9);
    for (int i = 0; i < 9; i++) b[a[i]] = i;
    return b;
}
int sym(vi mp, int pos)
{
    int ans = 0;
    for (int i = 0; i < 9; i++) ans += (pos%pow3[i+1]/pow3[i])*pow3[mp[i]];
    return ans;
}
//symmetry map that yields min value when applied
piv normalize(int pos)
{
    if (pos == 19683) return piv(19683,vi());
    //symmetry map applied that yields the normalized position
    piv best(19683,vi());
    for (int i = 0; i < 4; i++) for (int j = 0; j <= 1; j++)
    {
        vi mp = {0,1,2,3,4,5,6,7,8};
        for (int k = 0; k < i; k++) mp = cmp(mp,rCW);
        if (j) mp = cmp(mp,rLR);
        if (sym(mp,pos) < best.first) best = piv(sym(mp,pos),mp);
    }
    return best;
}
//whether previous player won already; player to move: 0 = X, 1 = O
bool won(int pos, int player)
{
    for (vi t: lines)
    {
        bool won = true;
        for (int i : t) if (pos%pow3[i+1]/pow3[i] != 2-player) won = false;
        if (won) return true;
    }
    return false;
}
//if draw then special position 19683
vi moves(int pos)
{
    int player = 0;
    for (int i = 0; i < 9; i++) if (pos%pow3[i+1] >= pow3[i]) player = 1-player;
    if (won(pos,player)) return vi();
    vi ans;
    for (int i = 0; i < 9; i++) if (pos%pow3[i+1] < pow3[i])
    {
        int npos = pos+pow3[i]*(player+1);
        ans.push_back(npos);
    }
    if (ans.empty()) return vi(1,19683);
    return ans;
}
//ainode winprob[0] = X, [1] = O; not implemented
//positions are not normalized
struct ainode
{
    vector<pid> v;
    double winprob[2] = {0,0};
} ai[19684];
int aimove(int pos)
{
    double total = 0;
    for (pid p : ai[pos].v) total += p.second;
    if (total == 0) return -1;
    total = rand()*total/RAND_MAX;
    for (int i = 0; i < ai[pos].v.size(); i++)
    {
        total -= ai[pos].v[i].second;
        if (total < 0) return ai[pos].v[i].first;
    }
}
void winprob(int pos)
{
    //fix this position: unimplemented
}
char getchar(int& pos, int i)
{
    int t = pos;
    pos /= 3;
    if (t%3 == 0) return char('0'+i);
    if (t%3 == 1) return 'X';
    return 'O';
}
void display(int pos)
{
/*
   |   |   
 1 | 2 | 3 
   |   |   
-----------
   |   |   
 4 | 5 | 6 
   |   |   
-----------
   |   |   
 7 | 8 | 9 
   |   |   
*/
    int cur = pos;
    for (int i = 0; i <= 2; i++)
    {
        cout << "   |   |   \n ";
        cout << getchar(cur,i*3+1);
        cout << " | ";
        cout << getchar(cur,i*3+2);
        cout << " | ";
        cout << getchar(cur,i*3+3);
        cout << " \n   |   |   \n";
        if (i != 2) cout << "-----------\n";
    }
}
void load()
{
    string savefile;
    cout << "Enter data file location, '!' to cancel: ";
    cin >> savefile;
    if (savefile == "!") return;
    ifstream data(savefile);
    for (int i = 0; i < 19683; i++) ai[i].v.clear();
    while (true)
    {
        int a, b;
        data >> a;
        if (a == -1) break;
        double c;
        data >> b >> c;
        if (b < 0)
        {
            if (b == -1) ai[a].winprob[0] = c;
            else ai[a].winprob[1] = c;
        }
        else ai[a].v.push_back(pid(b,c));
    }
    data.close();
    cout << "Data loaded\n";
}
void save()
{
    string savefile;
    cout << "Enter data file location: ";
    cin >> savefile;
    ofstream data(savefile);
    for (int i = 0; i < 19683; i++) if (i == normalize(i).first)
    {
        data << i << ' ' << -1 << ' ' << ai[i].winprob[0] << '\n';
        data << i << ' ' << -2 << ' ' << ai[i].winprob[1] << '\n';
        for (pid move : ai[i].v)
            data << i << ' ' << move.first << ' ' << move.second << '\n';
    }
    data << -1;
    data.close();
    cout << "Data saved\n";
}