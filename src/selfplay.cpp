#include "common.h"

void initialise() {
    cout << "won, drawn, lost, invalid, known states / 5" << endl;
}

void selfplay() {
    static int num_games;
    num_games++;
    GameState g;
    vector<pair<GameState, Output>> history;
    while (not g.terminated()) {
        auto probs = mcts(g, nnet);
        history.push_back({g, {0, probs}});  // evaluation to be filled in later
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
    for (int i = 0; i < TRAINING_BATCH_SIZE; i++) {
        auto [g, o] = buffer.sample();
        nnet.train(Image(g), o);
    }
}

void evaluation() {
    auto result = evaluate([](GameState g) -> int {
        auto probs = nnet.predict(Image(g)).policy;
        // auto probs = mcts(g, nnet);
        return sample(probs);
    });
    auto [completed, invalid] = result;
    auto [won, drawn] = completed;
    int lost = NUM_EVALUATE - (won + drawn);
    cout << won << ", " << drawn << ", " << lost << ", " << invalid << ", "
         << nnet.known_states() / 5 << endl;
}

void mainLoop() {
    int game_count = 0;
    while (game_count != MAX_SELF_PLAY_GAMES) {
        // approximation for running jobs in parallel
        for (int i = 0; i < EVALUATION_STEPS; i++)
            evaluation();
        for (int i = 0; i < SELFPLAY_STEPS; i++)
            selfplay();
        for (int i = 0; i < TRAINING_STEPS; i++)
            training();
        // nnet.dump_to_file();
        game_count++;
    }
}