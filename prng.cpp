#include "prng.hpp"

/**
 * Seeds the pseudo-random number generator.
 */
PRNG::PRNG(unsigned int s) {
    seed = s;
}

/**
 * Produces the next pseudo-random number in the sequence.
 */
unsigned int PRNG::Next() {
    seed = 214013 * seed + 2531011;
    return seed;
}
