#include "basic_entities.hpp"

pr::Entity::Entity( pr::Model &model ) : model( model ) {

}

pr::Model &pr::Entity::getModel() {
    return model;
}

void pr::Entity::draw( pr::Shader &shader ) {
    glm::mat4 M;
    M = Scalable::transform( Rotatable::transform( Positionable::transform( M ) ) );
    shader.setUniform( "M", M );
//    TODO
//    shader.setUniform( "material.ambient", x );
//    shader.setUniform( "material.diffuse", x );
//    glm::vec3 spec = glm::vec3( 1.0, 1.0, 1.0 );
//    shader.setUniform( "material.specular", spec );
//    float shi = 100.0;
//    shader.setUniform( "material.shininess", shi );
//    shader.setAttrib( "iPos", 4, myCubeVertices );
//    shader.setAttrib( "iTexCoord", 2, myCubeTexCoords );
//    shader.setAttrib( "iNormal", 4, myCubeNormals );
//    glm::mat3 normalMatrix = glm::transpose( glm::inverse( glm::mat3( M )));
//    shader.setUniform( "M", M );
//    shader.setUniform( "normalMatrix", normalMatrix );
//    shader.draw( {"iPos", "iTexCoord", "iNormal"}, GL_TRIANGLES, myCubeVertexCount );
}
