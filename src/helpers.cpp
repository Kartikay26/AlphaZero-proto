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

int sample(const array<float, MAX_ACTIONS> &a)
{
    float f = randomFloat(0, 1);
    float c = 0;
    for (int i = 0; i < MAX_ACTIONS; i++)
    {
        c += a[i];
        if (f <= c)
        {
            return i;
        }
    }
    assert(false); // execution shouldn't reach here
}

string spaces(int n, char c)
{
    string ans = "";
    for (int i = 0; i < n; i++)
    {
        ans += c;
    }
    return ans;
}