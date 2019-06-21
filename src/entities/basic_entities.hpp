#ifndef CG_BASIC_ENTITIES_HPP
#define CG_BASIC_ENTITIES_HPP

#include "graphics/modeling/model.hpp"
#include "object_properties.hpp"

namespace pr {

    class Entity : public Positionable, public Rotatable, public Scalable, public Parentable {

        Model* model;
        const Parentable* parent = nullptr;

    public:

        glm::mat4 M = glm::mat4(1);

        Entity();

        Entity( Model &model );

        void setParent( const Parentable& entity );

        glm::mat4 modelMatrix() const;

        Model* getModel() const;

        void draw( Shader &shader ) const;

        bool operator==( const Entity& ) const;

    };

}

namespace std {
    template<>
    struct hash< pr::Entity > {
        size_t operator()( const pr::Entity &obj ) const;
    };
}
#endif //CG_BASIC_ENTITIES_HPP
