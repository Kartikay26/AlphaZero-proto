#include "common.h"

// ==================================================================

// config

const int NUM_EVALUATE = 1000;
const int BUFFER_SIZE = 10000;
const int TRAINING_BATCH_SIZE = 1;

// relative speeds
const int SELFPLAY_SPEED = 1;
const int TRAINING_SPEED = 1;
const int EVALUATION_SPEED = 1;

// ==================================================================

NeuralNet nnet;
ReplayBuffer buffer(BUFFER_SIZE);

ofstream clog("logfile.txt");

void initialise()
{
    cout << "won, drawn, invalid" << endl;
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
        int action = randomMove(g);         // sample(probs);
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
    auto [g, o] = buffer.sample();
    nnet.train(Image(g), o);
}

void evaluation()
{
    auto result = evaluate([](GameState g) -> int {
        return sample(nnet.predict(Image(g)).policy);
    });
    auto [completed, invalid] = result;
    auto [won, drawn] = completed;
    cout << won << ", "
         << drawn << ", "
         << invalid << endl;
}

int main()
{
    initialise();

    while (true)
    {
        // approximation for running jobs in parallel
        for (int i = 0; i < SELFPLAY_SPEED; i++)
            selfplay();
        for (int i = 0; i < TRAINING_SPEED; i++)
            training();
        for (int i = 0; i < EVALUATION_SPEED; i++)
            evaluation();
    }
}

// ==================================================================
