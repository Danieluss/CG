#ifndef CG_BASIC_ENTITIES_HPP
#define CG_BASIC_ENTITIES_HPP

#include "graphics/modeling/model.hpp"
#include "object_properties.hpp"

namespace pr {

    class Entity : public Positionable, public Rotatable, public Scalable {

        Model &model;

    public:

        Entity( Model &model );

        Model& getModel();

        void draw( Shader &shader );
    };

}

#endif //CG_BASIC_ENTITIES_HPP
