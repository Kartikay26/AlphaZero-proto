#pragma once

#include <bits/stdc++.h>

const int BOARD_SIZE = 9;
const int MAX_ACTIONS = 9;

// ==================================================================
// ========================== tictactoe.cpp =========================

class GameState;
enum class Square;
enum class Player;
enum class Outcome;

// ========================== neuralnet.cpp =========================

class NeuralNet;
class Image;
class Output;

// ========================== selfplay.cpp ==========================

// ============================ mcts.cpp ============================

// ==================================================================
// ==================================================================

// ========================== tictactoe.cpp =========================

class GameState
{
private:
    Square board[BOARD_SIZE];

    friend class Image;

public:
    GameState(); // root of game tree
    Player turn();
    bool checkPlayerWon(Player p);
    Outcome evaluate();
    bool terminated();
    std::array<bool, MAX_ACTIONS> possibleActions();
    GameState playAction(int action);

    friend std::ostream &operator<<(std::ostream &out, GameState g); // print
    std::string toString();
};

// ==================================================================

enum class Square
{
    first = 1,
    second = -1,
    empty = 0,
};

char represent(Square sq);

enum class Player
{
    first = int(Square::first),
    second = int(Square::second),
};

Player opponent(Player p);

enum class Outcome
{
    running,
    draw,
    first_won = int(Player::first),
    second_won = int(Player::second),
};

bool isTerminal(Outcome o);

// ========================== neuralnet.cpp =========================

class NeuralNet
{
};

// ==================================================================

class Image
{
public:
    double img[3][3];

    Image(GameState g);
};

class Output
{
public:
    double evaluation;
    double policy[9];
};

// ========================== selfplay.cpp ==========================

// ============================ mcts.cpp ============================

// ==================================================================