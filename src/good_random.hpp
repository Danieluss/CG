#ifndef CG_GOODRANDOM_HPP
#define CG_GOODRANDOM_HPP

#include <random>

namespace pr {
    class GoodRandom {
        int a;
        std::mt19937 _rng;
        std::uniform_real_distribution< double > dist;

    public:
        static GoodRandom rng;
        GoodRandom();
        double get();
        int getInt( int a, int b );
        int getInt( int b ); // <a; b)
    };
}


#endif //CG_GOODRANDOM_HPP
