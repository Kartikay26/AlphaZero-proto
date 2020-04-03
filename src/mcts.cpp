#include "common.h"

array<float, MAX_ACTIONS> mcts(GameState g, NeuralNet nnet)
{
    return nnet.predict(Image(g)).policy;
}