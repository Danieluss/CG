#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include "object_properties.hpp"

namespace pr {

    void Parentable::setParent( const pr::Parentable &entity ) {
//    std::cout<<"PARENTING"<<std::endl;
        parent = &entity;
    }

    void Positionable::translate( const glm::vec3 &offset ) {
        pos += offset;
    }

    void Positionable::translate( const double &x, const double &y, const double &z ) {
        pos += glm::vec3{x, y, z};
    }

    glm::mat4 Positionable::transform( glm::mat4 matrix ) const {
        return glm::translate( matrix, pos );
    }

    void Rotatable::rotate( const float &angle, const Axis &ax ) {
        rot[ax] += angle;
        rot[ax] -= 2 * M_PI * ( (int) ( rot[ax] / M_PI / 2 ) );
    }

    glm::mat4 Rotatable::transform( glm::mat4 matrix ) const {
        glm::quat q = glm::quat( rot );
        return matrix*glm::mat4_cast( q );
    }

    void Rotatable::rotateD( const float &angleD, const Axis& axis ) {
        rotate( angleD * M_PI / 180.0, axis );
    }

    void Scalable::scale( const double &x, const double &y, const double &z ) {
        scale( {x, y, z} );
    }

    void Scalable::scale( const glm::vec3 &scale ) {
        scale_ = scale_ * scale;
    }

    glm::mat4 Scalable::transform( glm::mat4 matrix ) const {
        return glm::scale( matrix, scale_ );
    }
}