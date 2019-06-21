#ifndef CG_CAMERA_HPP
#define CG_CAMERA_HPP

#include "object_properties.hpp"
#include "min_max_property.hpp"

namespace pr {

    struct Camera : public Parentable {

        MinMaxProperty< float > speed{1.f/128.f, 128.f, 4.f};
        MinMaxProperty< float > mouseSensitivity{1.f/128.f, 128.f, 0.25f};
        MinMaxProperty< float > zoom_{1.f/32.f, 1.f, 1.f};

        float fov = 70;
        float minDist = 0.1;
        float maxDist = 200;
        float ratio = 1.77777777778;

        float yaw = 0;
        MinMaxProperty< float > pitch{-89.f, 89.f, 0.f};
        Positionable position;
        glm::vec3 dir{1, 0, 0};
        glm::vec3 nose{0, 0, 1};

        bool locked = false;

        void move( const float& time, MoveDir dir );

        glm::mat4 modelMatrix() const;

        glm::mat4 view() const;

        glm::mat4 projection();

        void zoom( const float &d );

        void turn( glm::vec2 vec );

        void accelerate( const float &d );

        Camera();

        void viewOf( const Camera& camera );

    protected:

        virtual void moveLeft( const float &time );

        virtual void moveRight( const float &time );

        virtual void moveForward( const float &time );

        virtual void moveBackward( const float &time );

        virtual void moveUp( const float &time );

        virtual void moveDown( const float &time );

    };

    struct UFOCamera : public Camera {

    protected:

        void moveLeft( const float &time ) override;

        void moveRight( const float &time ) override;

        void moveForward( const float &time ) override;

        void moveBackward( const float &time ) override;

        void moveUp( const float &time ) override;

        void moveDown( const float &time ) override;

    };

}

#endif //CG_CAMERA_HPP
