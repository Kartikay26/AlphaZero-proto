#pragma once

// from stdlib

#include <bits/stdc++.h>

using std::accumulate;
using std::any_of;
using std::array;
using std::cout;
using std::deque;
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

const int BOARD_SIZE = 9;
const int MAX_ACTIONS = 9;

extern const int NUM_EVALUATE;
extern const int BUFFER_SIZE;
extern const int TRAINING_BATCH_SIZE;

extern const int SELFPLAY_STEPS;
extern const int TRAINING_STEPS;
extern const int EVALUATION_STEPS;

extern const int NUM_SIMULATIONS;
extern const float C_PUCT;

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

void initialise();
void mainLoop();

// ==================================================================

// globals

extern NeuralNet nnet;
extern ReplayBuffer buffer;

extern ofstream clog;

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

// Fake neural net, for now

class NeuralNet {
   private:
    map<string, Output> trained;

   public:
    Output predict(Image i);
    void train(Image i, Output o);
    int known_states() { return trained.size(); }
    void dump_to_file();
};

class Image {
    friend class NeuralNet;

   private:
    string hash;

   public:
    Image(GameState g);
};

class Output {
   public:
    float evaluation;
    array<float, MAX_ACTIONS> policy;

    friend ostream& operator<<(ostream& out, Output& g);  // print
};

// ========================== evaluate.cpp ==========================

int randomMove(GameState g);

Outcome playGame(function<int(GameState)> player1,
                 function<int(GameState)> player2);

pair<pair<int, int>, int> evaluate(function<int(GameState g)> selectMove);

// =========================== helpers.cpp ==========================

int randomInt(int min, int max);
float randomFloat(float min, float max);

int sample(const array<float, MAX_ACTIONS>& a);

string spaces(int n, char c = ' ');

// ============================ mcts.cpp ============================

array<float, MAX_ACTIONS> mcts(GameState g, NeuralNet& nnet);

float simulate(GameState s, NeuralNet& nnet, int depth = 0);

array<float, MAX_ACTIONS> uniform(GameState g, NeuralNet& nnet);

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