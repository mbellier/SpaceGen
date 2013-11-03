#ifndef PRNG_H
#define PRNG_H

class PRNG {
private:
    unsigned int seed;
public:
    PRNG(unsigned int s);
    unsigned int Next();
};

#endif // PRNG_H
