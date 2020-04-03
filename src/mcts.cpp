#include "common.h"

array<float, MAX_ACTIONS> mcts(GameState g, NeuralNet nnet)
{
    auto actions = g.getPossibleActions();
    int num_actions = accumulate(actions.begin(), actions.end(), 0);
    array<float, MAX_ACTIONS> probs;
    for (int i = 0; i < MAX_ACTIONS; i++)
    {
        if (actions[i])
        {
            probs[i] = 1. / num_actions;
        }
        else
        {
            probs[i] = 0;
        }
    }
    return probs;
}