#include "common.h"

// ==================================================================

// relative speeds
const int SELFPLAY_SPEED = 0;
const int TRAINING_SPEED = 0;
const int EVALUATION_SPEED = 1;

NeuralNet n;

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
    while (not g.terminated())
    {
        auto probs = mcts(g, n);
        int action = sample(probs);
        g = g.playAction(action);
    }
}

void training()
{
}

void evaluation()
{
    auto result = evaluate([](GameState g) -> int {
        return randomInt(0, 8);
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
