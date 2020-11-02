#include "common.h"

using MiniDNN::FullyConnected;
using MiniDNN::ReLU;
using MiniDNN::RegressionMSE;

NeuralNet::NeuralNet() {
    net.add_layer(new FullyConnected<ReLU>(9, 10));
    net.add_layer(new FullyConnected<ReLU>(10, 10));
    net.init(0, 0.1);
    opt.m_lrate = 0.1;
    MiniDNN::VerboseCallback callback;
    net.set_callback(callback);
    net.set_output(new RegressionMSE());
}

Output::Output(Eigen::VectorXd vec) {
    for (int i = 0; i < 9; i++)
    {
        policy[i] = vec[i];
    }
    evaluation = vec[9];
}

Output NeuralNet::predict(Image i) {
    Eigen::VectorXd v = net.predict(i.mat);
    return Output(v);
}

Eigen::VectorXd  Output::toVec() {
    Eigen::VectorXd vec(10);
    for (int i = 0; i < 9; i++) {
        vec[i] = policy[i];
    }
    vec[9] = evaluation;
    return vec;
}

void NeuralNet::train(Image i, Output o) {
    auto x = i.mat;
    auto y = o.toVec();
    // net.fit(opt, x, y, 1, 1);
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