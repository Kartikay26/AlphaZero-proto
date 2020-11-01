#include "common.h"

int randomInt(int min, int max) {
    return min + rand() % ((max + 1) - min);
}

double randomDouble(double min, double max) {
    double r = double(rand()) / double(RAND_MAX);
    return min + r * (max - min);
}

int sample(const array<double, MAX_ACTIONS>& a) {
    double eps = 0.0001;
    double f = randomDouble(0+eps, 1-eps);
    double c = 0;
    for (int i = 0; i < MAX_ACTIONS; i++) {
        c += a[i];
        if (f < c) {
            return i;
        }
    }
    assert(false);  // execution shouldn't reach here
}

string spaces(int n, char c) {
    string ans = "";
    for (int i = 0; i < n; i++) {
        ans += c;
    }
    return ans;
}