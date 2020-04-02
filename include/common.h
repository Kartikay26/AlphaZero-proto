#pragma once

// from stdlib

#include <bits/stdc++.h>

using std::set;
using std::map;
using std::string;
using std::cout;
using std::endl;
using std::vector;
using std::array;
using std::ostream;
using std::ostringstream;
using std::any_of;

// constants / config

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

// ========================== tictactoe.cpp =========================

class GameState
{
    friend class Image;

private:
    Square board[BOARD_SIZE];

public:
    GameState(); // root of game tree
    Player turn();
    bool checkPlayerWon(Player p);
    Outcome evaluate();
    bool terminated();
    array<bool, MAX_ACTIONS> possibleActions();
    GameState playAction(int action);

    friend ostream &operator<<(ostream &out, GameState g); // print
    string toString();
};

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

// Fake neural net, for now

class NeuralNet
{
private:
    map<string, Output> trained;

public:
    Output predict(Image i);
    void train(Image i, Output o);
};

class Image
{
    friend class NeuralNet;

private:
    string str;

public:
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