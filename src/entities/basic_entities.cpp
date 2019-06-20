#include "basic_entities.hpp"

pr::Entity::Entity( pr::Model &model ) : model( model ) {

}

pr::Model &pr::Entity::getModel() const {
    return model;
}

glm::mat4 pr::Entity::modelMatrix() const {
    glm::mat4 M( 1.0 );
    if( parent != nullptr ) {
        M = parent->modelMatrix();
    }
    return Scalable::transform( Rotatable::transform( Positionable::transform( M ) ) );
}

void pr::Entity::draw( pr::Shader &shader ) const {
    glm::mat4 M = modelMatrix();
    shader.setUniform( "M", M );
    glm::mat3 normalMatrix = glm::inverse( glm::mat3( M ));
    shader.setUniform( "normalMatrix", normalMatrix );
    model.draw( shader );
}

void pr::Entity::setParent( const pr::Parentable &entity ) {
    parent = &entity;
}
