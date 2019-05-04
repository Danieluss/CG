#include <glm/gtc/matrix_transform.hpp>
#include "models/object_properties.hpp"
#include "object_properties.hpp"


void Positionable::translate( const glm::vec3 &offset ) {
    position += offset;
}

void Positionable::translate( const double &x, const double &y, const double &z ) {
    position += glm::vec3{ x, y, z };
}

void Positionable::transform( glm::mat4& matrix ) {
    matrix = glm::translate( matrix, position );
}

Positionable::operator glm::vec3() {
    return position;
}

void Rotatable::rotate( const double &angleD, const Axis& ax ) {
    glm::vec3 axis;
    if( ax == X ) {
        axis = { 1, 0, 0 };
    } else if( ax == Y ) {
        axis = { 0, 1, 0 };
    } else if( ax == Z ) {
        axis = { 0, 0, 1 };
    }
    rotate( angleD, axis );
}

void Rotatable::rotate( const double &angleD, glm::vec3 axis ) {
    rotation = glm::rotate( rotation, static_cast<float>( angleD*M_PI/180.0 ), axis );
}

void Rotatable::transform( glm::mat4 &matrix ) {
    matrix = rotation * matrix;
}

Rotatable::operator glm::mat4() {
    return rotation;
}

void Scalable::scale( const double &x, const double &y, const double &z ) {
    scale( { x, y, z } );
}

void Scalable::scale( const glm::vec3 &scale ) {
    scale_ = scale_ * scale;
}

void Scalable::transform( glm::mat4 &matrix ) {
    matrix = glm::translate( matrix, scale_ );
}

Scalable::operator glm::vec3() {
    return scale_;
}
