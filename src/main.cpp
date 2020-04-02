#include "common.h"

void recurse(GameState g, int i = 0);

int main()
{
    GameState g;
    recurse(g);
}

void recurse(GameState g, int i)
{
    for (int k = 0; k < i; k++)
    {
        std::cout << " ";
    }

    std::cout << g << std::endl;

    auto acts = g.possibleActions();

    for (int k = 0; k < MAX_ACTIONS; k++)
    {
        if (acts[k])
        {
            recurse(g.playAction(k), i + 1);
        }
    }
}