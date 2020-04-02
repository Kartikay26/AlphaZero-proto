#include "common.h"

int randomInt(int min, int max)
{
    return min + rand() % ((max + 1) - min);
}

float randomFloat(float min, float max)
{
    float r = float(rand()) / float(RAND_MAX);
    return min + r * (max - min);
}