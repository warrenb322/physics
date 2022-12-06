#include "tictactoe_ai.h"
int initw[10] = {50,20,8,4,3,2,2,1,1,1};
int main()
{
    //every meaningful move is equally likely
    for (int pos = 0; pos < 19683; pos++) if (pos == normalize(pos).first)
    {
        //X-O = 0 or 1
        int x = 0, o = 0;
        for (int i = 0; i < 9; i++)
        {
            int t = pos%pow3[i+1]/pow3[i];
            if (t == 1) x++;
            else if (t == 2) o++;
        }
        if (x != o and x != o+1) continue;
        //initial weight depends on no. cells total:
        for (int move : moves(pos))
            ai[pos].v.push_back(pid(move,initw[x+o]));
    }
    //winprobs of 19683 are 0, of won positions are 1 for one side
    //other positions: recursion; not implemented
    save();
}