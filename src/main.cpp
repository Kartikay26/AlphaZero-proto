#include "common.h"

// ==================================================================

// config

const int NUM_EVALUATE = 1000;
const int BUFFER_SIZE = 10000;
const int TRAINING_BATCH_SIZE = 100;
const int MAX_SELF_PLAY_GAMES = 10000;

const int NUM_SIMULATIONS = 20;
const double C_PUCT = 1.25;

const int SELFPLAY_STEPS = 10;
const int TRAINING_STEPS = 1;
const int EVALUATION_STEPS = 0;

// ==================================================================

// globals

NeuralNet nnet;
ReplayBuffer buffer(BUFFER_SIZE);

ofstream log_f("logfile.txt");

// ==================================================================

int main() {
    // srand(time(NULL));
    
    cout << fixed << setprecision(5);
    
    mainLoop();
}

// ==================================================================
