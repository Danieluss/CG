#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <GL/glew.h>
#include "camera.hpp"

namespace pr {

    void Camera::move( const float &time, pr::MoveDir dir ) {
        if( !locked ) {
            if( dir == FORWARD )
                moveForward( time );
            else if( dir == BACKWARD )
                moveBackward( time );
            else if( dir == LEFT )
                moveLeft( time );
            else if( dir == RIGHT )
                moveRight( time );
            else if( dir == UP )
                moveUp( time );
            else if( dir == DOWN )
                moveDown( time );
        }
    }

    void Camera::moveLeft( const float &time ) {
        position.translate(( time*speed )*glm::normalize( glm::cross( nose, dir )));
    }

    void Camera::moveRight( const float &time ) {
        position.translate(( time*speed )*glm::normalize( glm::cross( dir, nose )));
    }

    void Camera::moveForward( const float &time ) {
        position.translate(( time*speed )*dir );
    }

    void Camera::moveBackward( const float &time ) {
        position.translate(( -time*speed )*dir );
    }

    void Camera::moveUp( const float &time ) {
        position.translate(( time*speed )*nose );
    }

    void Camera::moveDown( const float &time ) {
        position.translate(( -time*speed )*nose );
    }

    glm::mat4 Camera::modelMatrix() const {
        return glm::lookAt( position.val, position.val + dir, nose );
    }

    glm::mat4 Camera::projection( const float& ratio ) {
        return glm::perspective( zoom_*glm::radians( fov ), ratio, minDist, maxDist );
    }

    void Camera::zoom( const float &d ) {
        if( !locked ) {
            if( d < 0 ) {
                zoom_ = zoom_*2;
            } else if( d > 0 ) {
                zoom_ = zoom_/2;
            }
        }
    }

    void Camera::accelerate( const float &d ) {
        if( !locked ) {
            speed = speed*d;
        }
    }

    void Camera::turn( glm::vec2 vec ) {
        if( !locked ) {
            vec *= mouseSensitivity;
            yaw -= vec[0];
            yaw = std::fmod( yaw, 360.f );
            pitch = pitch - vec[1];

            dir.x = cos( glm::radians( yaw ))*cos( glm::radians( pitch.value ));
            dir.y = sin( glm::radians( yaw ))*cos( glm::radians( pitch.value ));
            dir.z = sin( glm::radians( pitch.value ));

            dir = glm::normalize( dir );
            glm::vec3 right = glm::normalize( glm::cross( dir, glm::vec3{0, 0, 1} ));
            nose = glm::normalize( glm::cross( right, dir ));
        }
    }

}