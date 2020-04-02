#include "common.h"

void recurse(GameState g, int i = 0);

set<string> s;

int main()
{
    GameState g;
    recurse(g);
    cout << s.size() << endl;
}

void recurse(GameState g, int i)
{
    // for (int k = 0; k < i; k++)
    // {
    //     cout << " ";
    // }

    // cout << g << endl;

    auto acts = g.possibleActions();

    for (int k = 0; k < MAX_ACTIONS; k++)
    {
        if (acts[k])
        {
            recurse(g.playAction(k), i + 1);
        }
    }

    s.insert(g.toString());
}