#include "common.h"

/// NEURALNET INIT IN MAIN

// NeuralNet::NeuralNet() ;

Output::Output(Matrix vec) {
    for (int i = 0; i < 9; i++)
    {
        policy[i] = vec(i, 0);
    }
    evaluation = vec(9, 0) * 2 - 0.5;
}

Output NeuralNet::predict(Image i) {
    Matrix v = net.predict(i.mat);
    return Output(v);
}

Matrix  Output::toMat() {
    Matrix vec(10, 1);
    for (int i = 0; i < 9; i++) {
        vec(i, 0) = policy[i];
    }
    vec(9, 0) = evaluation/2. + 0.5;
    return vec;
}

void NeuralNet::train(Matrix gamestates, Matrix outputs) {
    callback.previous_loss = -1;
    net.fit(opt, gamestates, outputs, TRAINING_BATCH_SIZE, TRAINING_EPOCHS);
    loss = net.get_output()->loss();
}

Image::Image(GameState g) : mat(9, 1) {
    hash = g.hash();
    mat << double(g.board[0]), double(g.board[1]), double(g.board[2]),
           double(g.board[3]), double(g.board[4]), double(g.board[5]),
           double(g.board[6]), double(g.board[7]), double(g.board[8]);
}

ostream& operator<<(ostream& out, Output& o) {
    out << o.evaluation << " [ ";
    for (double& p : o.policy) {
        out << p << " ";
    }
    out << "]";
    return out;
}

void NeuralNet::dump_to_file() {
    static int i = 0;
    i++;
    // ofstream fdump("nndump.txt");
    // net.export_net("nnet","dump_nnet");
}

// ==================================================================
