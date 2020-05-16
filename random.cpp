#include "random.h"

int Random::randint(int from, int to) {
        std::random_device dev;
        std::mt19937 rng(dev());
        std::uniform_int_distribution<std::mt19937::result_type> dist(from, to);

        return dist(rng);
}
