#ifndef CG_EVENTS_HPP
#define CG_EVENTS_HPP

#include <functional>
#include <vector>
#include <glm/glm.hpp>
#include <unordered_set>
#include <list>

namespace pr {

    template< typename T >
    struct ExpandLambda {};

    template< typename Ret, typename Class, typename... Args >
    struct ExpandLambda< Ret( Class::* )( Args... ) const > {
        using Type = std::function< Ret( Args... ) >;
    };

    template< typename F >
    typename ExpandLambda< decltype( &F::operator()) >::Type acceptFunction( F const &func ) {
        return func;
    }

    template< class Ret, class... Param >
    std::function<Ret(Param...)> acceptFunction( Ret(*func)( Param... )) {
        return func;
    }



    template< class T >
    using TriggerRule = std::function< bool( T ) >;

    template< class... T >
    class Trigger {

    protected:

        template< class V >
        bool _add( int n, V &&v ) {
            return false;
        }

        bool triggered() const {
            return true;
        }

    };

    template< class T, class... TArgs >
    class Trigger< T, TArgs... > : Trigger< TArgs... > {
        using Super = Trigger< TArgs... >;
        std::vector< TriggerRule< T > > trigger;

    protected:

        template< typename E >
        struct id {
            typedef E type;
        };

        template< class V >
        bool _add( int n, TriggerRule< V > &&v ) {
            if( n > 0 ) {
                return Super::_add( n - 1, std::forward< TriggerRule< V > >( v ));
            } else {
                trigger.push_back( v );
                return true;
            }
        }

    public:

        template< class V >
        bool add( int n, V &&v ) {
            return _add( n, acceptFunction( v ) );
        }

        bool triggered( T t, TArgs... args ) const {
            bool acc = true;
            for( auto it = trigger.begin(); it != trigger.end(); it++ ) {
                acc &= ( *it )( t );
            }
            return acc && Super::triggered( args... );
        }
    };

    struct ButtonEvent {
        int key, action, mods;
    };

    class ButtonTrigger : public Trigger< int, int, int > {
        using Super = Trigger< int, int, int >;

    public:

        bool triggered( const ButtonEvent &event ) const {
            return Super::triggered( event.key, event.action, event.mods );
        }

        bool triggered( int key, int action, int mods ) const {
            return Super::triggered( key, action, mods );
        }

        enum Pos {
            KEY, ACTION, MODS
        };

    };

//    template< class T >
//    struct Point {
//        T x, y;
//    };

    class MotionTrigger : public Trigger< glm::vec2 > {

        using Super = Trigger< glm::vec2 >;

    public:

        bool triggered( glm::vec2 vec ) const {
            return Super::triggered( vec );
        }

        bool triggered( double x, double y ) const {
            return Super::triggered( { x, y } );
        }

    };

    template< class T >
    class TriggerBuilder {
    };

    template<>
    class TriggerBuilder< ButtonTrigger > {
        using BttnTrig = ButtonTrigger;
        using Type = TriggerBuilder< BttnTrig >;
        ButtonTrigger trigger;

    public:

        ButtonTrigger get() {
            return trigger;
        }

        Type &key( const int& key ) {
            return keys( { key } );
        }

        Type &keys( const std::list< int > &keys ) {
            std::unordered_set< int > keySet;
            for( int key : keys ) {
                keySet.insert( key );
            }
            trigger.add( BttnTrig::KEY, [ keySet ]( int a ) -> bool { return keySet.find( a ) != keySet.end(); } );
            return *this;
        }

        Type &action( const int& action ) {
            return actions( { action } );
        }

        Type &actions( const std::list< int > &actions ) {
            std::unordered_set< int > actionSet;
            for( int action : actions ) {
                actionSet.insert( action );
            }
            trigger.add( BttnTrig::ACTION, [ actionSet ]( int a ) -> bool { return actionSet.find( a ) != actionSet.end(); } );
            return *this;
        }

        Type &modsXor( const int& mods ) {
            trigger.add( BttnTrig::MODS, [ mods ]( int a ) -> bool { return ( a ^ mods ) > 0; } );
            return *this;
        }

        Type &modsAnd( const int& mods ) {
            trigger.add( BttnTrig::MODS, [ mods ]( int a ) -> bool { return ( a & mods ) > 0; } );
            return *this;
        }

        Type &mods( const int& mods ) {
            trigger.add( BttnTrig::MODS, [ mods ]( int a ) -> bool { return mods == a; } );
            return *this;
        }

        template< class V >
        Type& customKey( V&& keyRule ) {
            trigger.add( BttnTrig::KEY, keyRule );
            return *this;
        }

        template< class V >
        Type& customMod( V&& modRule ) {
            trigger.add( BttnTrig::MODS, modRule );
            return *this;
        }

        template< class V >
        Type& customAction( V&& actionRule ) {
            trigger.add( BttnTrig::ACTION, actionRule );
            return *this;
        }

    };

    template<>
    class TriggerBuilder< MotionTrigger > {
        using MtnTrig = MotionTrigger;
        using Type = TriggerBuilder< MtnTrig >;
        MtnTrig trigger;

    public:

        MtnTrig get() {
            return trigger;
        }

        Type& sameDir( glm::vec2 dir, double maxAngle ) {
            dir = glm::normalize( dir );
            trigger.add( 0, [dir, maxAngle]( glm::vec2 vec ) -> bool {
                vec = glm::normalize( vec );
                return glm::acos( glm::dot(vec, dir) ) <= maxAngle;
            } );
            return *this;
        }

        Type& otherDir( glm::vec2 dir, double minAngle ) {
            dir = glm::normalize( dir );
            trigger.add( 0, [dir, minAngle]( glm::vec2 vec ) -> bool {
                vec = glm::normalize( vec );
                return glm::acos( glm::dot(vec, dir) ) >= minAngle;
            } );
            return *this;
        }

        Type& inSquare( glm::vec2 beg, glm::vec2 dim ) {
            trigger.add( 0, [beg, dim]( glm::vec2 vec ) -> bool {
                return vec[0] >= beg[0]
                        && vec[0] <= beg[0] + dim[0]
                        && vec[1] >= beg[1]
                        && vec[1] <= beg[1] + dim[1];
            } );
            return *this;
        }

        Type& outSquare( glm::vec2 beg, glm::vec2 dim ) {
            trigger.add( 0, [beg, dim]( glm::vec2 vec ) -> bool {
                return ! ( vec[0] >= beg[0]
                       && vec[0] <= beg[0] + dim[0]
                       && vec[1] >= beg[1]
                       && vec[1] <= beg[1] + dim[1] );
            } );
            return *this;
        }

        Type& inCircle( glm::vec2 middle, double radius ) {
            trigger.add( 0, [middle, radius]( glm::vec2 vec ) -> bool {
                return glm::distance( middle, vec ) <= radius;
            } );
            return *this;
        }

        Type& outCircle( glm::vec2 middle, double radius ) {
            trigger.add( 0, [middle, radius]( glm::vec2 vec ) -> bool {
                return glm::distance( middle, vec ) > radius;
            } );
            return *this;
        }

        template< class V >
        Type& custom( V&& rule ) {
            trigger.add( 0, rule );
            return *this;
        }

    };

    template< class T >
    TriggerBuilder< T > build() {
        return {};
    }
}

#endif //CG_EVENTS_HPP
