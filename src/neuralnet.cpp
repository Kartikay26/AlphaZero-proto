#include "common.h"

Output NeuralNet::predict(Image i)
{
    if (trained.count(i.str))
    {
        return trained[i.str];
    }
    else
    {
        Output o;
        o.evaluation = 0;
        for (float &p : o.policy)
        {
            p = 1 / 9.;
        }
        return o;
    }
}

void NeuralNet::train(Image i, Output o)
{
    trained[i.str] = o;
}

Image::Image(GameState g)
{
    str = g.toString();
}

// ==================================================================