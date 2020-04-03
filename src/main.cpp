#include "common.h"

// ==================================================================

// config

const int NUM_EVALUATE = 1000;
const int BUFFER_SIZE = 10000;
const int TRAINING_BATCH_SIZE = 1000;

const int NUM_SIMULATIONS = 10;
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

int main()
{
    // initialise();
    // mainLoop();

    GameState g;
    auto result = mcts(g, nnet);
    for (auto &x : result)
    {
        cout << x << endl;
    }
}

// ==================================================================
