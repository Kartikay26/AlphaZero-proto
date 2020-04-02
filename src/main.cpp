#include "common.h"

set<string> s;

// ==================================================================

int main()
{
    srand(time(NULL));

    while (true)
    {
        auto [completed, invalid] = evaluate([](GameState g) -> int {
            return randomMove(g);
        });

        auto [won, drawn] = completed;

        cout << won << " " << drawn
             << " " << invalid << endl;
    }
}

// ==================================================================

// ==================================================================
