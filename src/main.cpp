#include "common.h"

// ==================================================================

// config

const int NUM_EVALUATE = 1000;
const int BUFFER_SIZE = 10000;
const int TRAINING_BATCH_SIZE = 1000;

// relative speeds

const int SELFPLAY_SPEED = 10;
const int TRAINING_SPEED = 1;
const int EVALUATION_SPEED = 1;

// ==================================================================

int main()
{
    initialise();
    mainLoop();
}

// ==================================================================
