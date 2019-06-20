#ifndef CG_BASIC_ENTITIES_HPP
#define CG_BASIC_ENTITIES_HPP

#include "graphics/modeling/model.hpp"
#include "object_properties.hpp"

namespace pr {

    class Entity : public Positionable, public Rotatable, public Scalable, public Parentable {

        Model &model;
        const Parentable* parent = nullptr;

    public:

        Entity( Model &model );

        void setParent( const Parentable& entity );

        glm::mat4 modelMatrix() const;

        Model& getModel() const;

        void draw( Shader &shader, glm::mat4 M) const;
    };

}

#endif //CG_BASIC_ENTITIES_HPP
