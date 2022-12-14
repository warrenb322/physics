//WINPROBS (NOT IMPLEMENTED): t NOT b; QUIT: p NOT q
#include "tictactoe_ai.h"
int best[19683];
//0 if losing, 1 if drawing, 2 if winning for the player to move
vector<int> bestmove[19683];
int perfect(int pos)
{
    //guaranteed that the game is not drawn or lost yet
    return bestmove[pos][rand()%bestmove[pos].size()];
}
int main()
{
    //setup perfect player
    for (int i = 19682; i >= 0; i--)
    {
        vector<int> m = moves(i);
        if (m.empty()) best[i] = 0;
        else if (m[0] == 19683) best[i] = 1;
        else
        {
            best[i] = 0;
            for (int t : m) best[i] = max(best[i],2-best[t]);
            for (int t : m) if (best[t] == 2-best[i]) bestmove[i].push_back(t);
        }
    }
    srand(time(NULL));
    load();
    while (true)
    {
        string c;
        cout << "Play: b\nLoad: l\nSave: s\nStat: n\nAuto: a\nQuit: q\n";
        cin >> c;
        if (c == "b" or c == "t" or c == "n" or c == "a")
        {
            if (c == "t") cout << "Not implemented yet\n";
            int pos = 0, playercol = rand()%2+1, curplayer = 1;
            if (c == "a")
                cout << (playercol == 1 ? "Perfect: X\n" : "Perfect: O\n");
            int winner = 0;
            vi positions;
            while (true)
            {
                positions.push_back(pos);
                if (won(pos,curplayer-1))
                {
                    winner = 3-curplayer;
                    break;
                }
if (curplayer == playercol)
{
    bool draw = true;
    for (int i = 0; i < 9; i++) if (pos%pow3[i+1] < pow3[i]) draw = false;
    if (draw) break;
    if (c == "t") cout << "Win chance: " << ai[pos].winprob << '\n';
    if (c != "a") display(pos);
    if (c == "a") pos = perfect(pos);
    else while (true)
    {
        cout << ": ";
        string s;
        cin >> s;
        if (s.size() != 1) continue;
        int move = s[0]-'1';
        if (move < 0 or move >= '9') continue;
        if (pos%pow3[move+1] >= pow3[move]) continue;
        pos += pow3[move]*playercol;
        break;
    }
}
else
{
    piv norm = normalize(pos);
    int move = aimove(norm.first);
    if (move == -1)
    {
        winner = playercol;
        cout << "AI resigns\n";
        break;
    }
    if (move == 19683) break;
    piv npos = normalize(move);
    pos = sym(inv(norm.second),sym(inv(npos.second),npos.first));
}
                curplayer = 3-curplayer;
            }
            display(pos);
            if (winner) cout << ((winner == playercol)?"You win\n":"You lose\n");
            else cout << "Draw\n";
            if (c == "b" or c == "a")
            {
if (!winner) for (int i = 0; i < positions.size()-1; i++)
{
    int I = normalize(positions[i]).first, I1 = normalize(positions[i+1]).first;
    for (int j = 0; j < ai[I].v.size(); j++)
        if (I1 == normalize(ai[I].v[j].first).first)
            ai[I].v[j].second += 1;
}
else for (int i = 0; i < positions.size()-1; i++)
{
    int I = normalize(positions[i]).first, I1 = normalize(positions[i+1]).first;
    if (i%2 != winner%2)
    {
        for (int j = 0; j < ai[I].v.size(); j++)
            if (I1 == normalize(ai[I].v[j].first).first)
                ai[I].v[j].second += 3;
    }
    else
    {
        for (int j = 0; j < ai[I].v.size(); j++)
            if (I1 == normalize(ai[I].v[j].first).first)
            {
                ai[I].v[j].second -= 1;
                if (ai[I].v[j].second < 0.1) ai[I].v[j].second = 0.1;
            }
    }
}
            }
        }
        else if (c == "l") load();
        else if (c == "s") save();
        else if (c == "p")
        {
            cout << "Successfully terminated\n";
            return 0;
        }
        else cout << "Invalid input\n";
    }
}
/*
10 a's
a a a a a a a a a a
30 a's
a a a a a a a a a a a a a a a a a a a a a a a a a a a a a a
100 a's
a a a a a a a a a a a a a a a a a a a a a a a a a a a a a a a a a a a a a a a a a a a a a a a a a a a a a a a a a a a a a a a a a a a a a a a a a a a a a a a a a a a a a a a a a a a a a a a a a a a a
*/