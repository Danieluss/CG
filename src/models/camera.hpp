#ifndef CG_CAMERA_HPP
#define CG_CAMERA_HPP

#include "object_properties.hpp"
#include "min_max_property.hpp"

struct Camera {

    MinMaxProperty< float > speed{ 1.f/32.f, 32.f, 1.f };
    MinMaxProperty< float > mouseSensitivity{ 1.f/128.f, 128.f, 0.25f };
    MinMaxProperty< float > zoom_{ 1.f/32.f, 1.f, 1.f };

    float fov = 70;
    float minDist = 0.1;
    float maxDist = 100;

    float yaw = 0;
    MinMaxProperty< float > pitch{ -85.f, 85.f, 0.f };
    Positionable position;
    glm::vec3 dir{1, 0, 0};
    glm::vec3 nose{0, 0, 1};

    void moveLeft( const float &time );

    void moveRight( const float &time );

    void moveFront( const float &time );

    void moveBack( const float &time );

    void moveUp( const float &time );

    void moveDown( const float &time );

    glm::mat4 view();

    glm::mat4 projection();

    void zoom( const float &d );

    void turn( glm::vec2 vec );

    void accelerate( const float& d );
};

#endif //CG_CAMERA_HPP
