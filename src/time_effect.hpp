#ifndef CG_TIME_EFFECT_HPP
#define CG_TIME_EFFECT_HPP

#include <functional>
#include <vector>

namespace pr {

    struct TimeEffect {

        TimeEffect();

        void relative( double startTime );

        void setEvents( std::vector< std::pair<double, std::function<void()>> > );

        bool play( double time );

    private:

        std::vector< std::pair<double, std::function<void()>> > events;
        double refTime = -1;
        double recentTime = 0;
        std::vector< std::pair<double, std::function<void()>> >::iterator it;

    };

}

#endif //CG_TIME_EFFECT_HPP
