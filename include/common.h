#pragma once

// from stdlib

#include <bits/stdc++.h>

using std::accumulate;
using std::any_of;
using std::array;
using std::cout;
using std::endl;
using std::exception;
using std::function;
using std::map;
using std::ofstream;
using std::ostream;
using std::ostringstream;
using std::pair;
using std::set;
using std::string;
using std::vector;

// constants

#include "config.h"

// ==================================================================
// ========================== tictactoe.cpp =========================

class GameState;
class InvalidMove;
enum class Square;
enum class Player;
enum class Outcome;

// ========================== neuralnet.cpp =========================

class NeuralNet;
class Image;
class Output;

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
    array<bool, MAX_ACTIONS> getPossibleActions();
    GameState playAction(int action);

    friend ostream &operator<<(ostream &out, GameState &g); // print
    string toString();
};

class InvalidMove : exception
{
};

enum class Square
{
    first,
    second,
    empty,
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
    first_won = int(Player::first),
    second_won = int(Player::second),
    running,
    draw,
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
    float evaluation;
    array<float, MAX_ACTIONS> policy;
};

// ========================== evaluate.cpp ==========================

int randomMove(GameState g);

Outcome playGame(function<int(GameState)> player1, function<int(GameState)> player2);

pair<pair<int, int>, int> evaluate(function<int(GameState g)> selectMove);

// =========================== helpers.cpp ==========================

int randomInt(int min, int max);
float randomFloat(float min, float max);

int sample(const array<float, MAX_ACTIONS> &a);

// ============================ mcts.cpp ============================

array<float, MAX_ACTIONS> mcts(GameState g, NeuralNet n);

// ==================================================================