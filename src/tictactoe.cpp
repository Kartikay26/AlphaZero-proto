#include "common.h"

GameState::GameState()
{
    for (Square &sq : board)
    {
        sq = Square::empty;
    }
}

GameState::GameState(string str)
{
    map<char, Square> mapping = {
        {'.', Square::empty},
        {'x', Square::first},
        {'o', Square::second},
    };
    // string: 0123456789012
    //         [xox|x.x|ooo]
    // board:  .012.345.678.
    map<int, int> pos_map = {
        // str pos to board pos
        // 0 - x
        {1, 0},
        {2, 1},
        {3, 2},
        // 4 - x
        {5, 3},
        {6, 4},
        {7, 5},
        // 8 - x
        {9, 6},
        {10, 7},
        {11, 8},
        // 9 - x
    };
    for (auto [s, b] : pos_map)
    {
        board[b] = mapping[str[s]];
    }
}

Player GameState::turn()
{
    int turns = 0;

    for (Square sq : board)
    {
        if (sq != Square::empty)
            turns++;
    }

    if (turns % 2 == 0)
    {
        return Player::first;
    }
    else
    {
        return Player::second;
    }
}

bool GameState::checkPlayerWon(Player p)
{
    vector<vector<int>> winningCombinations = {
        {0, 1, 2},
        {3, 4, 5},
        {6, 7, 8},
        {0, 3, 6},
        {1, 4, 7},
        {2, 5, 8},
        {0, 4, 8},
        {2, 4, 6},
    };

    for (vector<int> &v : winningCombinations)
    {
        bool all = true;
        for (int i : v)
        {
            if (board[i] != Square(p))
            {
                all = false;
                break;
            }
        }
        if (all)
        {
            return true;
        }
    }

    return false;
}

Outcome GameState::evaluate()
{
    if (checkPlayerWon(Player::first))
    {
        return Outcome::first_won;
    }
    else if (checkPlayerWon(Player::second))
    {
        return Outcome::second_won;
    }
    else
    {
        bool squares_remain = any_of(board, board + BOARD_SIZE, [=](Square sq) {
            return sq == Square::empty;
        });

        if (squares_remain)
        {
            return Outcome::running;
        }
        else
        {
            return Outcome::draw;
        }
    }
}

bool GameState::terminated()
{
    return isTerminal(evaluate());
}

array<bool, MAX_ACTIONS> GameState::getPossibleActions()
{
    array<bool, MAX_ACTIONS> actions;
    actions.fill(false);

    if (terminated())
    {
        return actions;
    }
    else
    {
        for (int i = 0; i <= BOARD_SIZE; i++)
        {
            if (board[i] == Square::empty)
            {
                actions[i] = true;
            }
        }
        return actions;
    }
}

GameState GameState::playAction(int action)
{
    GameState next = (*this);
    // assert(getPossibleActions()[action] == true);
    if (next.board[action] != Square::empty)
    {
        throw InvalidMove();
    }
    next.board[action] = Square(turn());
    return next;
}

ostream &operator<<(ostream &out, GameState &g)
{
    out << "[";

    for (int i = 0; i < 3; i++)
        out << represent(g.board[i]);

    out << "|";

    for (int i = 3; i < 6; i++)
        out << represent(g.board[i]);

    out << "|";

    for (int i = 6; i < 9; i++)
        out << represent(g.board[i]);

    out << "]";
    return out;
}

string GameState::hash()
{
    ostringstream out;
    out << (*this);
    return out.str();
}

char represent(Square sq)
{
    map<Square, char> mapping = {
        {Square::empty, '.'},
        {Square::first, 'x'},
        {Square::second, 'o'},
    };
    return mapping[sq];
}

Player opponent(Player p)
{
    switch (p)
    {
    case Player::first:
        return Player::second;

    case Player::second:
        return Player::first;
    }
}

bool isTerminal(Outcome o)
{
    return not(o == Outcome::running);
}

// ==================================================================