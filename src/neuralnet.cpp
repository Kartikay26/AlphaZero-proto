#include "common.h"

Output NeuralNet::predict(Image i) {
    if (trained.count(i.hash)) {
        return trained[i.hash];
    } else {
        Output o;
        o.evaluation = 0;
        for (double& p : o.policy) {
            p = 1. / MAX_ACTIONS;
        }
        return o;
    }
}

void NeuralNet::train(Image i, Output o) {
    trained[i.hash] = o;
}

Image::Image(GameState g) {
    hash = g.hash();
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
    ofstream fdump("nndump.txt");
    for (auto& [s, o] : trained) {
        fdump << s << ' ' << o << endl;
    }
}

// ==================================================================