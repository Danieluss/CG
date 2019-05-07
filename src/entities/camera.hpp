#ifndef CG_CAMERA_HPP
#define CG_CAMERA_HPP

#include "object_properties.hpp"
#include "min_max_property.hpp"

namespace pr {

    struct Camera {

        MinMaxProperty< float > speed{1.f/128.f, 128.f, 2.f};
        MinMaxProperty< float > mouseSensitivity{1.f/128.f, 128.f, 0.25f};
        MinMaxProperty< float > zoom_{1.f/32.f, 1.f, 1.f};

        float fov = 70;
        float minDist = 0.1;
        float maxDist = 100;

        float yaw = 0;
        MinMaxProperty< float > pitch{-89.f, 89.f, 0.f};
        Positionable position;
        glm::vec3 dir{1, 0, 0};
        glm::vec3 nose{0, 0, 1};

        bool locked = false;

        void move( const float& time, MoveDir dir );

        glm::mat4 view();

        glm::mat4 projection( const float& ratio );

        void zoom( const float &d );

        void turn( glm::vec2 vec );

        void accelerate( const float &d );

    private:

        void moveLeft( const float &time );

        void moveRight( const float &time );

        void moveForward( const float &time );

        void moveBackward( const float &time );

        void moveUp( const float &time );

        void moveDown( const float &time );

    };

}

#endif //CG_CAMERA_HPP
