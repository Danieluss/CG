#ifndef CG_VERTEX_HPP
#define CG_VERTEX_HPP
#include <glm/glm.hpp>

namespace pr {

    struct Vertex {

        glm::vec3 position,
                normal,
                tangent,
                bitangent;
        glm::vec2 uv;

    };

}

#endif //CG_VERTEX_HPP
