#include "common.h"

void ReplayBuffer::insert(GameState g, Output o) {
    if (queue.size() == size) {
        queue.pop_front();
    }

    queue.push_back({g, o});
}

pair<GameState, Output> ReplayBuffer::sample() {
    return queue[randomInt(0, queue.size() - 1)];
}
