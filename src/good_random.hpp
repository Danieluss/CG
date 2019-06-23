#ifndef CG_GOODRANDOM_HPP
#define CG_GOODRANDOM_HPP

#include <random>

namespace pr {
    class GoodRandom {
        int a;
        std::mt19937 rng;
        std::uniform_real_distribution< double > dist;

    public:
        GoodRandom() : dist( 0.0, 1.0 ) {}
        double get() {
            return dist( rng );
        }
    };
    extern GoodRandom RNG;
}


#endif //CG_GOODRANDOM_HPP
