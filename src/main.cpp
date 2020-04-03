#include "common.h"

set<string> s;

// ==================================================================

int main()
{
    srand(time(NULL));

    cout << "won drawn invalid" << endl;

    while (true)
    {
        auto [completed, invalid] = evaluate([](GameState g) -> int {
            return randomInt(0, 8);
        });

        auto [won, drawn] = completed;

        cout << won << " "
             << drawn << " "
             << invalid << endl;
    }
}

// ==================================================================

// ==================================================================
