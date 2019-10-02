#include"utilities.hpp"

int pr::step(float a) {
    if(a > 0.0) {
        return 1;
    } else {
        return 0;
    }
}

int pr::sgn(float a) {
    if(a > 0.0) {
        return 1;
    } else {
        return -1;
    }
}