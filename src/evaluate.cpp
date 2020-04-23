#include "common.h"

pair<pair<int, int>, int> evaluate(function<int(GameState g)> selectMove) {
    // play NUM_EVALUATE games against random agents
    // and return number of games won and drawn by our function as first element
    // returns number of games failed by invalid move in second element

    int games_won = 0;
    int games_drawn = 0;
    int games_invalid = 0;

    for (int i = 0; i < NUM_EVALUATE / 2; i++) {
        try {
            Outcome o = playGame(selectMove, randomMove);
            if (o == Outcome::first_won)
                games_won++;
            if (o == Outcome::draw)
                games_drawn++;
        } catch (InvalidMove e) {
            games_invalid++;
        }

        try {
            Outcome o = playGame(randomMove, selectMove);
            if (o == Outcome::second_won)
                games_won++;
            if (o == Outcome::draw)
                games_drawn++;
        } catch (InvalidMove e) {
            games_invalid++;
        }
    }

    return {{games_won, games_drawn}, games_invalid};
}

int randomMove(GameState g) {
    array<bool, MAX_ACTIONS> possible = g.getPossibleActions();
    int num_possible = accumulate(possible.begin(), possible.end(), 0);
    assert(num_possible > 0);
    int chosen = randomInt(1, num_possible);  // 1st choice to nth choice
    int seen = 0;
    for (int i = 0; i < MAX_ACTIONS; i++) {
        if (possible[i]) {
            seen++;
            if (chosen == seen) {
                return i;
            }
        }
    }
    assert(false);  // execution should not reach here
}

Outcome playGame(function<int(GameState)> player1,
                 function<int(GameState)> player2) {
    GameState g;
    int turn = 0;
    while (not g.terminated()) {
        turn++;
        function<int(GameState)> moveSelect =
            (turn % 2 == 1) ? player1 : player2;
        int a = moveSelect(g);
        g = g.playAction(a);
    }
    return g.evaluate();
}