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
    s.insert(g.toString());

    for (int a : g.getPossibleActions())
    {
        recurse(g.playAction(a), i + 1);
    }
}