#include "common.h"

void selfplay() {
    static int num_games;
    num_games++;
    GameState g;
    vector<pair<GameState, Output>> history;
    while (not g.terminated()) {
        auto probs = mcts(g, nnet);
        history.push_back({g, Output(0, probs)});  // evaluation to be filled in later
        int action = sample(probs);
        g = g.playAction(action);
        log_f << action;
    }

    Outcome game_outcome = g.evaluate();
    map<Outcome, char> mapping = {
        {Outcome::draw, '.'},
        {Outcome::first_won, 'x'},
        {Outcome::second_won, 'o'},
    };
    log_f << " -> " << mapping[game_outcome] << endl;

    if (game_outcome != Outcome::draw) {
        for (auto& [h, h_output] : history) {
            if (h.turn() == Player(game_outcome)) {
                h_output.evaluation = +1;
            } else {
                h_output.evaluation = -1;
            }
        }
    }
    for (auto& [g, o] : history) {
        buffer.insert(g, o);
    }
}

void training() {
    Matrix gamestates(9, TRAINING_BATCH_SIZE);
    Matrix outputs(10, TRAINING_BATCH_SIZE);
    for (int i = 0; i < TRAINING_BATCH_SIZE; i++) {
        auto [g, o] = buffer.sample();
        auto im = Image(g).mat;
        gamestates.col(i) = im;
        outputs.col(i) = o.toMat();
    }
    nnet.train(gamestates, outputs);
}

tuple<int, int, int, int> evaluation() {
    auto result = evaluate([](GameState g) -> int {
        auto probs = nnet.predict(Image(g)).policy;
        // auto probs = mcts(g, nnet);
        return sample(probs);
    });
    return result;
}
