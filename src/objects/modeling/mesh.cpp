#include "mesh.hpp"

pr::Mesh::Mesh( const std::vector< unsigned int > &indices,
                const std::vector< pr::Vertex > &vertices,
                const std::vector< pr::Texture > &textures )
        : indices( indices ), vertices( vertices ), textures( textures ) {
    init();
}

void pr::Mesh::init() {
    glGenVertexArrays( 1, &VAO );
    glGenBuffers( 1, &VBO );
    glGenBuffers( 1, &EBO );

    glBindVertexArray( VAO );
    glBindBuffer( GL_ARRAY_BUFFER, VBO );
    glBufferData( GL_ARRAY_BUFFER, vertices.size()*sizeof( Vertex ), &vertices[0], GL_STATIC_DRAW );

    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, EBO );
    glBufferData( GL_ELEMENT_ARRAY_BUFFER, indices.size()*sizeof( unsigned int ), &indices[0], GL_STATIC_DRAW );

    glEnableVertexAttribArray( 0 );
    glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, sizeof( Vertex ), ( void * ) 0 );
    glEnableVertexAttribArray( 1 );
    glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, sizeof( Vertex ), ( void * ) offsetof( Vertex, normal ));
    glEnableVertexAttribArray( 2 );
    glVertexAttribPointer( 2, 2, GL_FLOAT, GL_FALSE, sizeof( Vertex ), ( void * ) offsetof( Vertex, uv ));
    glEnableVertexAttribArray( 3 );
    glVertexAttribPointer( 3, 3, GL_FLOAT, GL_FALSE, sizeof( Vertex ), ( void * ) offsetof( Vertex, tangent ));
    glEnableVertexAttribArray( 4 );
    glVertexAttribPointer( 4, 3, GL_FLOAT, GL_FALSE, sizeof( Vertex ), ( void * ) offsetof( Vertex, bitangent ));

    glBindVertexArray( 0 );
}

void pr::Mesh::draw( pr::Shader shader ) {
    unsigned diffuseId = 0;
    unsigned specularId = 0;
    unsigned normalId = 0;
    unsigned heightId = 0;
    for( unsigned int i = 0; i < textures.size(); i++ ) {
        glActiveTexture( GL_TEXTURE0 + i );
        std::string number;
        TexType type = textures[i].type;
//        std::string name = std::string( strTexType( type ) ); //sic!
        std::string name;
        if( type == DIFFUSE ) {
            name = "diffuse";
            number = std::to_string( diffuseId++ );
        } else if( type == SPECULAR ) {
            name = "specular";
            number = std::to_string( specularId++ );
        } else if( type == NORMAL ) {
            name = "normal";
            number = std::to_string( normalId++ );
        } else if( type == HEIGHT ) {
            name = "height";
            number = std::to_string( heightId++ );
        }

        glUniform1i( glGetUniformLocation( shader.getId(), ( name + number ).c_str() ), i );
        glBindTexture( GL_TEXTURE_2D, textures[i].id );
    }

    glBindVertexArray( VAO );
    glDrawElements( GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0 );
    glBindVertexArray( 0 );
    glActiveTexture( GL_TEXTURE0 );
}
