#include "time_effect.hpp"
#include <iostream>

bool pr::TimeEffect::play( double time ) {
    for( auto it = this->it; it != events.end() && it->first < time - refTime; ++it ) {
        it->second();
        ++this->it;
    }
    return it == events.end();
}

pr::TimeEffect::TimeEffect() {
}

void pr::TimeEffect::relative( double startTime ) {
    refTime = startTime;

}

void pr::TimeEffect::setEvents( std::vector< std::pair< double, std::function< void()>> > ev ) {
    events = ev;
    it = events.begin();
}
