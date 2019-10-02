#include <limits>
#include "good_random.hpp"

pr::GoodRandom pr::GoodRandom::rng = pr::GoodRandom();

double pr::GoodRandom::get() {
    return dist( _rng );
}

pr::GoodRandom::GoodRandom() : dist( 0.0, 1.0 - std::numeric_limits<double>::min() ) {

}

int pr::GoodRandom::getInt( int a, int b ) {
    return (int) get() * ( b - a ) + a;
}

int pr::GoodRandom::getInt( int b ) {
    return (int) get() * b;
}
