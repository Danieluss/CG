#include <glm/gtc/matrix_transform.hpp>
#include "object_properties.hpp"

namespace pr {

    void Positionable::translate( const glm::vec3 &offset ) {
        val += offset;
    }

    void Positionable::translate( const double &x, const double &y, const double &z ) {
        val += glm::vec3{x, y, z};
    }

    Positionable::operator glm::vec3() {
        return val;
    }

    glm::mat4 Positionable::transform( glm::mat4 matrix ) {
        return glm::translate( matrix, val );
    }

    void Rotatable::rotate( const double &angleD, const Axis &ax ) {
        glm::vec3 axis;
        if( ax == X ) {
            axis = {1, 0, 0};
        } else if( ax == Y ) {
            axis = {0, 1, 0};
        } else if( ax == Z ) {
            axis = {0, 0, 1};
        }
        rotate( angleD, axis );
    }

    void Rotatable::rotate( const double &angleD, glm::vec3 axis ) {
        rotation = glm::rotate( rotation, static_cast<float>( angleD*M_PI/180.0 ), axis );
    }

    glm::mat4 Rotatable::transform( glm::mat4 matrix ) {
        return rotation*matrix;
    }

    Rotatable::operator glm::mat4() {
        return rotation;
    }

    glm::vec3 Rotatable::eulerAngles() {

        float sy = sqrt(rotation[0][0] * rotation[0][0] + rotation[1][0] * rotation[1][0] );

        bool singular = sy < 1e-6; // If

        float x, y, z;
        if (!singular)
        {
            x = atan2(rotation[2][1] , rotation[2][2]);
            y = atan2(-rotation[2][0], sy);
            z = atan2(rotation[1][0], rotation[0][0]);
        }
        else
        {
            x = atan2(-rotation[1][2], rotation[1][1]);
            y = atan2(-rotation[2][0], sy);
            z = 0;
        }
        return glm::vec3(x, y, z);

    }

    void Scalable::scale( const double &x, const double &y, const double &z ) {
        scale( {x, y, z} );
    }

    void Scalable::scale( const glm::vec3 &scale ) {
        scale_ = scale_*scale;
    }

    glm::mat4 Scalable::transform( glm::mat4 matrix ) {
        return glm::scale( matrix, scale_ );
    }

    Scalable::operator glm::vec3() {
        return scale_;
    }

}