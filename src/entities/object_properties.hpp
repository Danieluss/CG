#ifndef CG_MODEL_PROPERTIES_HPP
#define CG_MODEL_PROPERTIES_HPP

#include "glm/glm.hpp"

namespace pr {

    struct Transformable {
        virtual void transform( glm::mat4 &matrix ) = 0;
    };

    struct Positionable : public Transformable {

        glm::vec3 val{0, 0, 0};

        operator glm::vec3();

        void transform( glm::mat4 &matrix );

        virtual void translate( const glm::vec3 &offset );

        virtual void translate( const double &x, const double &y = 0, const double &z = 0 );

    };

    enum Axis {
        X, Y, Z
    };

    struct Rotatable : public Transformable {

        glm::mat4 rotation;

        operator glm::mat4();

        void transform( glm::mat4 &matrix ) override;

        glm::vec3 eulerAngles();

        virtual void rotate( const double &angleD, glm::vec3 axis );

        virtual void rotate( const double &angleD, const Axis &axis );

    };

    struct Scalable : public Transformable {

        glm::vec3 scale_{1, 1, 1};

        operator glm::vec3();

        void transform( glm::mat4 &matrix ) override;

        virtual void scale( const glm::vec3 &scale );

        virtual void scale( const double &x, const double &y = 1, const double &z = 1 );

    };

}
#endif //CG_MODEL_PROPERTIES_HPP
