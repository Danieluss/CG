#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include "camera.hpp"

void Camera::moveLeft( const float &time ) {
    position.translate( ( time * speed ) * glm::normalize( glm::cross( nose, dir ) ) );
}

void Camera::moveRight( const float &time ) {
    position.translate( ( time * speed ) * glm::normalize( glm::cross( dir, nose ) ) );
}

void Camera::moveFront( const float &time ) {
    position.translate( ( time * speed ) * dir );
}

void Camera::moveBack( const float &time ) {
    position.translate( ( - time * speed ) * dir );
}

void Camera::moveUp( const float &time ) {
    position.translate( ( time * speed ) * nose );
}

void Camera::moveDown( const float &time ) {
    position.translate( ( - time * speed ) * nose );
}

glm::mat4 Camera::view() {
    return glm::lookAt( position.val, position.val + dir, nose );
}

glm::mat4 Camera::projection() {
    return glm::perspective( zoom_ * glm::radians( fov ), 1.78f, minDist, maxDist );
}

void Camera::zoom( const float &d ) {
    if( d < 0 ) {
        zoom_ = zoom_ * 2;
    } else if( d > 0 ) {
        zoom_ = zoom_ / 2;
    }
}

void Camera::accelerate( const float &d ) {
    speed = speed * d;
}

void Camera::turn( glm::vec2 vec ) {
    vec *= mouseSensitivity;
    yaw -= vec[0];
    yaw = std::fmod( yaw, 360.f );
    pitch = pitch - vec[1];

    dir.x = cos( glm::radians( yaw ) ) * cos( glm::radians( pitch.value ) );
    dir.y = sin( glm::radians( yaw ) ) * cos( glm::radians( pitch.value ) );
    dir.z = sin( glm::radians( pitch.value ) );

    dir = glm::normalize( dir );
    glm::vec3 right = glm::normalize( glm::cross( dir, glm::vec3{ 0, 0, 1 } ) );
    nose = glm::normalize( glm::cross( right, dir ) );
}
