#ifndef CG_LISTENERMANAGER_HPP
#define CG_LISTENERMANAGER_HPP

#include "window.hpp"
#include "input_triggers.hpp"
#include "observer.hpp"
#include <unordered_map>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

namespace std {

    template<>
    struct hash< pr::Stamped > {
        size_t operator()( const pr::Stamped &obj ) const {
            return hash< long >()( obj.getId());
        }
    };

    template<>
    struct equal_to< pr::Stamped > {
        bool operator()( const pr::Stamped &objA, const pr::Stamped &objB ) const {
            return objA.getId() == objB.getId();
        }
    };

    template<>
    struct equal_to< pr::MotionObserver > : public equal_to< pr::Stamped >{};

    template<>
    struct equal_to< pr::ButtonObserver > : public equal_to< pr::Stamped >{};

    template<>
    struct hash< pr::MotionObserver > : public hash< pr::Stamped > {};

    template<>
    struct hash< pr::ButtonObserver > : public hash< pr::Stamped > {};

}

namespace pr {

    class ListenerManager {

        std::unordered_map< int, std::unordered_set< ButtonObserver > > buttonObserverMap;
        std::unordered_set< MotionObserver > motionObservers;
        std::unordered_set< MotionObserver > mousePositionObservers;
        std::unordered_set< MotionObserver > scrollObservers;
        std::unordered_set< ButtonObserver > buttonObservers;

        void _keyNotify( int key, int action, int mods, std::unordered_set< ButtonObserver > &observers ) {
            for( auto it = observers.begin(); it != observers.end(); it++ ) {
                it->update( key, action, mods );
            }
        }

        ListenerManager() {}
        double lastX = -1, lastY = -1;

    public:

        ListenerManager( const ListenerManager & ) = delete;
        void operator=( const ListenerManager & ) = delete;

        static ListenerManager& instance() {
            static ListenerManager instance;
            return instance;
        }

        void keyNotify( int key, int action, int mods ) {
            _keyNotify( key, action, mods, buttonObserverMap[key] );
            _keyNotify( key, action, mods, buttonObservers );
        }

        void positionNotify( double x, double y ) {
            for( auto it = mousePositionObservers.begin(); it != mousePositionObservers.end(); it++ ) {
                it->update( x, y );
            }
            double dx = x - lastX;
            double dy = y - lastY;
            if( x != -1 && ( dx != 0 || dy != 0 )) {
                for( auto it = motionObservers.begin(); it != motionObservers.end(); it++ ) {
                    it->update( dx, dy );
                }
            }
            lastX = x;
            lastY = y;
        }

        void scrollNotify( double x, double y ) {
            for( auto it = scrollObservers.begin(); it != scrollObservers.end(); it++ ) {
                it->update( x, y );
            }
        }

        static void keyNotify( GLFWwindow *window, int key, int, int action, int mods ) {
            instance().keyNotify( key, action, mods );
        }

        static void mouseKeyNotify( GLFWwindow *window, int button, int action, int mods ) {
            instance().keyNotify( button, action, mods );
        }

        static void mousePositionNotify( GLFWwindow *window, double x, double y ) {
            instance().positionNotify( x, y );
        }

        static void mouseScrollNotify( GLFWwindow *window, double x, double y ) {
            instance().scrollNotify( x, y );
        }

        void hook( Window &window ) {
            glfwSetKeyCallback( window, ListenerManager::keyNotify );
            glfwSetMouseButtonCallback( window, ListenerManager::mouseKeyNotify );
            glfwSetCursorPosCallback( window, ListenerManager::mousePositionNotify );
            glfwSetScrollCallback( window, ListenerManager::mouseScrollNotify );
        }

        void remove( int key, ButtonObserver observer ) {
            if( buttonObserverMap.find( key ) != buttonObserverMap.end()) {
                buttonObserverMap[key].erase( buttonObserverMap[key].find( observer ));
            }
        }

        void remove( ButtonObserver observer ) {
            buttonObservers.erase( buttonObservers.find( observer ) );
        }

        void remove( MotionObserver observer ) {
            motionObservers.erase( motionObservers.find( observer ) );
            scrollObservers.erase( scrollObservers.find( observer ) );
        }

        void addMappedButtonObs( int key, ButtonObserver observer ) {
            if( buttonObserverMap.find( key ) == buttonObserverMap.end()) {
                buttonObserverMap.insert( std::make_pair( key, std::unordered_set< ButtonObserver >()));
            }
            buttonObserverMap[key].insert( observer );
        }

        void addButtonObs( ButtonObserver observer ) {
            buttonObservers.insert( observer );
        }

        void addMouseScrollObs( MotionObserver observer ) {
            scrollObservers.insert( observer );
        }

        void addMouseMotionObs( MotionObserver observer ) {
            motionObservers.insert( observer );
        }

        void addMousePositionObs( MotionObserver observer ) {
            mousePositionObservers.insert( observer );
        }

    };

}

#endif //CG_LISTENERMANAGER_HPP
