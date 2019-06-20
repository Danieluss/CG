#include "mesh.hpp"

#define ENUM_STRINGS

pr::Mesh::Mesh( const std::vector< unsigned int > &indices,
                const std::vector< pr::Vertex > &vertices,
                const std::vector< pr::Texture > &textures )
        : indices( indices ), vertices( vertices ), textures( textures ) {
}

void pr::Mesh::draw( pr::Shader &shader ) {
    unsigned diffuseId = 0;
    unsigned specularId = 0;
    unsigned normalId = 0;
    unsigned heightId = 0;
    shader.setUniform("material.vambient", ambient);
    shader.setUniform("material.vdiffuse", diffuse);
    shader.setUniform("material.vspecular", specular);
    shader.setUniform("material.shininess", shininess);
    for( unsigned int i = 0; i < textures.size(); i++ ) {
        std::string number;
        TexType type = textures[i].type;
        std::string name;
        glm::vec3 empty = glm::vec3(-1,-1,-1);
        if( type == DIFFUSE ) {
            name = "diffuse";
            number = std::to_string( diffuseId++ );
        } else if( type == SPECULAR ) {
            name = "specular";
            number = std::to_string( specularId++ );
        } else if( type == NORMAL ) {
            name = "normal";
            number = std::to_string( normalId++ );
        } else if( type == AMBIENT ) {
            name = "ambient";
            number = std::to_string( heightId++ );
        }
        textures[i].activate(i);
        shader.setUniform(("material."+name).c_str(), i);
        shader.setUniform(("material.v"+name).c_str(), empty);
    }
    std::vector<float> pos, tex, norm;
    for(Vertex &v : vertices) {
        for(int i=0; i < 3; i++) {
            pos.push_back(v.position[i]);
            norm.push_back(v.normal[i]);
        }
        for(int i=0; i < 2; i++) {
            tex.push_back(v.uv[i]);
        }
    }
    shader.setAttrib("iPos", 3, pos.data());
    shader.setAttrib("iTexCoord", 2, tex.data());
    shader.setAttrib("iNormal", 3, norm.data());
    shader.draw( {"iPos", "iNormal", "iTexCoords"}, GL_TRIANGLES, indices.size(), indices.data());
}

pr::Mesh::Mesh() {
//    TODO
//    init(); causes errors when other textures are loaded
}

#undef ENUM_STRINGS