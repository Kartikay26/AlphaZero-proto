#include "common.h"

void initialise()
{
    srand(time(NULL));
    cout << "won, drawn, lost, invalid" << endl;
}

void selfplay()
{
    static int num_games;
    num_games++;
    clog << "Game #" << num_games << endl;
    GameState g;
    vector<pair<GameState, Output>> history;
    while (not g.terminated())
    {
        auto probs = mcts(g, nnet);
        history.push_back({g, {0, probs}}); // evaluation to be filled in later
        int action = sample(probs);
        g = g.playAction(action);
    }
    Outcome game_outcome = g.evaluate();
    if (game_outcome != Outcome::draw)
    {
        for (auto &[h, h_output] : history)
        {
            if (h.turn() == Player(game_outcome))
            {
                h_output.evaluation = +1;
            }
            else
            {
                h_output.evaluation = -1;
            }
        }
    }
    for (auto &[g, o] : history)
    {
        buffer.insert(g, o);
        clog << g << " -> " << o << endl;
    }
}

void training()
{
    for (int i = 0; i < TRAINING_BATCH_SIZE; i++)
    {
        auto [g, o] = buffer.sample();
        nnet.train(Image(g), o);
    }
}

void evaluation()
{
    auto result = evaluate([](GameState g) -> int {
        return sample(nnet.predict(Image(g)).policy);
    });
    auto [completed, invalid] = result;
    auto [won, drawn] = completed;
    int lost = NUM_EVALUATE - (won + drawn);
    cout << won << ", "
         << drawn << ", "
         << lost << ", "
         << invalid << endl;
}

void mainLoop()
{
    while (true)
    {
        // approximation for running jobs in parallel
        for (int i = 0; i < SELFPLAY_STEPS; i++)
            selfplay();
        for (int i = 0; i < TRAINING_STEPS; i++)
            training();
        for (int i = 0; i < EVALUATION_STEPS; i++)
            evaluation();
    }
}