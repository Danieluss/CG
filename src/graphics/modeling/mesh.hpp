#ifndef CG_MESH_HPP
#define CG_MESH_HPP
#define offset( structure, member ) ((size_t)((char *)&((structure *)0)->member - (char *)0))

#include <vector>
#include "graphics/shaders/shader.hpp"
#include "vertex.hpp"
#include "texture.hpp"

namespace pr {

    class Mesh {

        unsigned VBO, EBO;

        void init();

    public:

        std::vector< unsigned > indices;
        std::vector< Vertex > vertices;
        std::vector< Texture > textures;
        unsigned VAO;

        Mesh();

        Mesh( const std::vector< unsigned int > &indices, const std::vector< Vertex > &vertices,
              const std::vector< Texture > &textures );

        void draw( Shader shader );


    };

}

#endif //CG_MESH_HPP
