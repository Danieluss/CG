#include "mesh.hpp"

#include<iostream>
using namespace std;
#define ENUM_STRINGS

pr::Mesh::Mesh( const std::vector< unsigned int > &indices,
                const std::vector< pr::Vertex > &vertices,
                const std::vector< pr::Texture > &textures )
        : indices( indices ), vertices( vertices ), textures( textures ) {
}

void pr::Mesh::updateArrays() {
    positions.clear();
    normals.clear();
    texcoords.clear();
    for(Vertex &v : vertices) {
        for(int i=0; i < 3; i++) {
            positions.push_back(v.position[i]);
            normals.push_back(v.normal[i]);
        }
        texcoords.push_back(v.uv[0]);
        texcoords.push_back(1.0-v.uv[1]);
    }
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
            continue;
        } else if( type == AMBIENT ) {
            name = "ambient";
            number = std::to_string( heightId++ );
        }
        textures[i].activate(i);
        shader.setUniform(("material."+name).c_str(), i);
        shader.setUniform(("material.v"+name).c_str(), empty);
    }
    shader.setAttrib("iPos", 3, positions.data());
    shader.setAttrib("iTexCoord", 2, texcoords.data());
    shader.setAttrib("iNormal", 3, normals.data());
    shader.draw(GL_TRIANGLES, indices.size(), indices.data());
}

pr::Mesh::Mesh() {
}

#undef ENUM_STRINGS