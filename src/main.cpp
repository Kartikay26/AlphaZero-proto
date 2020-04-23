#include "common.h"

// ==================================================================

// config

const int NUM_EVALUATE = 1000;
const int BUFFER_SIZE = 10000;
const int TRAINING_BATCH_SIZE = 100;

const int NUM_SIMULATIONS = 20;
const float C_PUCT = 1.25;

const int SELFPLAY_STEPS = 10;
const int TRAINING_STEPS = 1;
const int EVALUATION_STEPS = 1;

// ==================================================================

// globals

NeuralNet nnet;
ReplayBuffer buffer(BUFFER_SIZE);

ofstream clog("logfile.txt");

// ==================================================================

void test(string s) {
    GameState g(s);
    cout << g << endl;
    auto result = mcts(g, nnet);
    float d = 0;
    for (auto& x : result) {
        cout << x << endl;
        d += x;
    }
    cout << "total: " << d << endl;
}

int main() {
    srand(time(NULL));

    initialise();
    mainLoop();
}

// ==================================================================
