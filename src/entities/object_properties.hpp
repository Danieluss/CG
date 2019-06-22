#ifndef CG_MODEL_PROPERTIES_HPP
#define CG_MODEL_PROPERTIES_HPP

#include "glm/glm.hpp"

namespace pr {

    enum MoveDir {
        FORWARD, BACKWARD, UP, DOWN, LEFT, RIGHT
    };

    struct Parentable {

        const Parentable* parent = nullptr;

        void setParent( const Parentable& entity );

        virtual glm::mat4 modelMatrix() const = 0;

    };

    struct Transformable {

//        virtual glm::mat4 transform( glm::mat4& matrix ) {
//            return transform( matrix );
//        }

    };

    struct Positionable : public Transformable {

        glm::vec3 pos{0, 0, 0};

        glm::mat4 transform( glm::mat4 matrix ) const;

        virtual void translate( const glm::vec3 &offset );

        virtual void translate( const double &x, const double &y = 0, const double &z = 0 );

    };

    enum Axis {
        X, Y, Z
    };

    struct Rotatable : public Transformable {

        glm::vec3 rot = {0, 0, 0};

        glm::mat4 transform( glm::mat4 matrix ) const;

        virtual void rotate( const float &angle, const Axis &axis );

        virtual void rotateD( const float &angleD, const Axis& axis );

    };

    struct Scalable : public Transformable {

        glm::vec3 scale_{1, 1, 1};

        glm::mat4 transform( glm::mat4 matrix ) const;

        virtual void scale( const glm::vec3 &scale );

        virtual void scale( const double &x, const double &y = 1, const double &z = 1 );

    };

}
#endif //CG_MODEL_PROPERTIES_HPP
