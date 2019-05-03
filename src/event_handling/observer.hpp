#ifndef CG_OBSERVER_HPP
#define CG_OBSERVER_HPP

#include "input_triggers.hpp"
#include <GL/glew.h>
#include <GLFW/glfw3.h>

namespace pr {

    class Stamped {
        long id;

        static long last;

    public:

        Stamped() {
            id = last;
            last++;
        }

        long getId() const {
            return id;
        }

        bool operator==( const Stamped &stamped ) {
            return id == stamped.id;
        }

    };

    template< class T, class... TArgs >
    class Observer : public Stamped {
        using Type = Observer<T, TArgs...>;
        using Callback = std::function< void( TArgs... ) >;

        T trig;
        Callback callb;

    public:

        Observer(){

        }

        template< class C >
        Observer( T&& trigger, C&& callback ) {
            callb = callback;
            trig = trigger;
        }

        template< class C >
        Type& callback( C&& callback ) {
            this->callb = acceptFunction( callback );
            return *this;
        }

        Type& trigger( const T& trigger ) {
            this->trig = trigger;
            return *this;
        }

        Type& trigger( T&& trigger ) {
            this->trig = trigger;
            return *this;
        }

        T& getTrigger() {
            return trig;
        }

        void update( TArgs... args ) const {
            if( trig.triggered( args... )) {
                callb( args... );
            }
        }

    };

    using ButtonObserver = Observer< ButtonTrigger, int, int, int >;

    using MotionObserver = Observer< MotionTrigger, double, double >;

}
#endif //CG_OBSERVER_HPP
