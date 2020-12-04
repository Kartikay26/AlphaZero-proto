#include "common.h"

// ==================================================================

// config

const int NUM_EVALUATE = 0;
const int BUFFER_SIZE = 10000;
const int TRAINING_BATCH_SIZE = 100;
const int TRAINING_EPOCHS = 10;
const double TRAINING_LEARNING_RATE = 0.5;
const int MAX_SELF_PLAY_GAMES = 10000;

const int NUM_SIMULATIONS = 20;
const double C_PUCT = 1.25;

const int SELFPLAY_STEPS = 1;
const int TRAINING_STEPS = 1;
const int EVALUATION_STEPS = 1;

// ==================================================================

// globals

NeuralNet nnet;
ReplayBuffer buffer(BUFFER_SIZE);

ofstream log_f("logfile.txt");

// ==================================================================

using MiniDNN::FullyConnected;
using MiniDNN::RegressionMSE;
using MiniDNN::ReLU;
using MiniDNN::Sigmoid;
using MiniDNN::Softmax;

NeuralNet::NeuralNet() {
    net.add_layer(new FullyConnected<Sigmoid>(9, 100));
    net.add_layer(new FullyConnected<Softmax>(100, 10));
    net.init(0, 0.1);
    opt.m_lrate = TRAINING_LEARNING_RATE;
    net.set_callback(callback);
    net.set_output(new MiniDNN::RegressionMSE());
}

int main() {
    // srand(time(NULL));
    cout << fixed << setprecision(5);
    // cout << "win, draw, lost, nloss" << endl;
    cout << "nloss, learn" << endl;
    int game_count = 0;
    while (game_count != MAX_SELF_PLAY_GAMES) {
        int won, drawn, lost, invalid;
        // approximation for running jobs in parallel
        for (int i = 0; i < EVALUATION_STEPS; i++)
            tie(won, drawn, lost, invalid) = evaluation();
        for (int i = 0; i < SELFPLAY_STEPS; i++)
            selfplay();
        for (int i = 0; i < TRAINING_STEPS; i++)
            training();
        nnet.dump_to_file();
        game_count++;
        // if (game_count % 1000 == 0) {
        //     nnet.opt.m_lrate /= 2;
        // }
        // cout << won << ", " << drawn << ", " << lost << ", "
        //      << nnet.loss * NUM_EVALUATE << endl;
    }
}

// ==================================================================
