#include "basic_entities.hpp"




pr::Entity::Entity() {

}

pr::Entity::Entity( pr::Model &model ) : model( &model ) {

}

pr::Model *pr::Entity::getModel() const {
    return model;
}

glm::mat4 pr::Entity::modelMatrix() const {
    glm::mat4 Mtmp = M;
    if( parent != nullptr ) {
        Mtmp = M * parent->modelMatrix();
    }
    return Scalable::transform( Rotatable::transform( Positionable::transform( Mtmp ) ) );
}

void pr::Entity::draw( pr::Shader &shader ) const {
    glm::mat4 M = modelMatrix();
    shader.setUniform( "M", M );
    glm::mat3 normalMatrix = glm::inverse( glm::mat3( M ));
    shader.setUniform( "normalMatrix", normalMatrix );
    model->draw( shader );
}



bool pr::Entity::operator==( const pr::Entity& entity ) const {
    return &entity == this;
}

namespace std {
    size_t hash< pr::Entity >::operator()( const pr::Entity &obj ) const {
        auto addr = reinterpret_cast<uintptr_t>( &obj );
        #if SIZE_MAX < UINTPTR_MAX
              addr %= SIZE_MAX;
        #endif
        return addr;
    }
}