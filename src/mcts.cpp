#include "common.h"

array<float, MAX_ACTIONS> mcts(GameState g, NeuralNet n)
{
    Output o = n.predict(Image(g));
    return o.policy;
}