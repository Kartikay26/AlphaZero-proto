#include "common.h"

GameState::GameState()
{
    for (Square &sq : board)
    {
        sq = Square::empty;
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
    std::vector<std::vector<int>> winningCombinations = {
        {0, 1, 2},
        {3, 4, 5},
        {6, 7, 8},
        {0, 3, 6},
        {1, 4, 7},
        {2, 5, 8},
        {0, 4, 8},
        {2, 4, 6},
    };

    for (std::vector<int> &v : winningCombinations)
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
        bool squares_remain = std::any_of(board, board + BOARD_SIZE, [=](Square sq) {
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

std::array<bool, MAX_ACTIONS> GameState::possibleActions()
{
    std::array<bool, MAX_ACTIONS> actions;
    actions.fill(false);

    if (terminated())
    {
        return actions;
    }
    else
    {
        for (int i = 0; i <= BOARD_SIZE; ++i)
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
    assert(possibleActions()[action] == true);
    next.board[action] = Square(turn());
    return next;
}

std::ostream &operator<<(std::ostream &out, GameState g)
{
    out << "[";

    for (int i = 0; i < 3; ++i)
        out << represent(g.board[i]);

    out << "|";

    for (int i = 3; i < 6; ++i)
        out << represent(g.board[i]);

    out << "|";

    for (int i = 6; i < 9; ++i)
        out << represent(g.board[i]);

    out << "]";
}

std::string GameState::toString(){
    std::ostringstream out;
    out << (*this);
    return out.str();
}

// ==================================================================

char represent(Square sq)
{
    std::map<Square, char> mapping = {
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