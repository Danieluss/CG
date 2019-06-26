#ifndef CG_UTILITIES_HPP
#define CG_UTILITIES_HPP

#include<utility>
#include<functional>

namespace pr {
    struct pair_hash {
        template <class T1, class T2>
        std::size_t operator () (const std::pair<T1,T2> &p) const {
            auto h1 = std::hash<T1>{}(p.first);
            auto h2 = std::hash<T2>{}(p.second);
            return h1 ^ h2;  
        }
    };

    int step(float a);

    int sgn(float a); 
}

#endif