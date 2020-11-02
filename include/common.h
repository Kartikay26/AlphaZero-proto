#pragma once

// from stdlib

#include <iostream>
#include <iomanip>
#include <fstream>
#include <random>
#include <time.h>
#include <string>
#include <sstream>
#include <algorithm>
#include <assert.h>
#include <numeric>
#include <array>
#include <map>
#include <set>
#include <deque>
#include <functional>

#include <MiniDNN/MiniDNN.h>

using namespace std;

// constants

const int BOARD_SIZE = 9;
const int MAX_ACTIONS = 9;

extern const int NUM_EVALUATE;
extern const int BUFFER_SIZE;
extern const int TRAINING_BATCH_SIZE;
extern const int MAX_SELF_PLAY_GAMES;

extern const int SELFPLAY_STEPS;
extern const int TRAINING_STEPS;
extern const int EVALUATION_STEPS;

extern const int NUM_SIMULATIONS;
extern const double C_PUCT;

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

// ========================= replaybuffer.cpp =======================

class ReplayBuffer;

// =========================== selfplay.cpp =========================

void mainLoop();

// ==================================================================

// globals

extern NeuralNet nnet;
extern ReplayBuffer buffer;

extern ofstream log_f;

// ========================== tictactoe.cpp =========================

class GameState {
    friend class Image;

   private:
    Square board[BOARD_SIZE];

   public:
    GameState();          // root of game tree
    GameState(string s);  // from string repr
    Player turn();
    bool checkPlayerWon(Player p);
    Outcome evaluate();
    bool terminated();
    array<bool, MAX_ACTIONS> getPossibleActions();
    GameState playAction(int action);

    friend ostream& operator<<(ostream& out, GameState& g);  // print
    string hash();
};

class InvalidMove : exception {};

enum class Square {
    first,
    second,
    empty,
};

char represent(Square sq);
Square toSquare(char c);

enum class Player {
    first = int(Square::first),
    second = int(Square::second),
};

Player opponent(Player p);

enum class Outcome {
    first_won = int(Player::first),
    second_won = int(Player::second),
    running,
    draw,
};

bool isTerminal(Outcome o);

// ========================== neuralnet.cpp =========================

typedef Eigen::MatrixXd Matrix;

class NeuralNet {
   private:
    MiniDNN::Network net;
    MiniDNN::SGD opt;
    MiniDNN::VerboseCallback callback;

   public:
    NeuralNet();
    Output predict(Image i);
    void train(Image i, Output o);
    void dump_to_file();
};

class Image {
   private:
    string hash;

   public:
    Matrix mat;
    Image(GameState g);
};

class Output {
   public:
    double evaluation;
    array<double, MAX_ACTIONS> policy;

    Output(double eval, array<double, MAX_ACTIONS> pol)
        : evaluation(eval), policy(pol) {};
    Output(Matrix vec);
    Matrix toMat();
    friend ostream& operator<<(ostream& out, Output& g);  // print
};

// ========================== evaluate.cpp ==========================

int randomMove(GameState g);

Outcome playGame(function<int(GameState)> player1,
                 function<int(GameState)> player2);

pair<pair<int, int>, int> evaluate(function<int(GameState g)> selectMove);

// =========================== helpers.cpp ==========================

int randomInt(int min, int max);
double randomDouble(double min, double max);

int sample(const array<double, MAX_ACTIONS>& a);

string spaces(int n, char c = ' ');

// ============================ mcts.cpp ============================

array<double, MAX_ACTIONS> mcts(GameState g, NeuralNet& nnet);

double simulate(GameState s, NeuralNet& nnet, int depth = 0);

array<double, MAX_ACTIONS> uniform(GameState g, NeuralNet& nnet);

// ========================= replaybuffer.cpp =======================

class ReplayBuffer {
   private:
    int size;
    deque<pair<GameState, Output>> queue;

   public:
    ReplayBuffer(int size) : size(size) {}

    void insert(GameState g, Output o);
    pair<GameState, Output> sample();
};

// ==================================================================