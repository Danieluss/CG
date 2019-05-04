#ifndef CG_DIRLIGHT_HPP
#define CG_DIRLIGHT_HPP

#include<glm/glm.hpp>
#include"shader.hpp"

namespace pr {
    class DirectionalLight {
        glm::vec3 direction;
        glm::vec3 ambient;
        glm::vec3 diffuse;
        glm::vec3 specular;
    public:
        DirectionalLight(glm::vec3 direction, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular) : 
            direction(direction), ambient(ambient), diffuse(diffuse), specular(specular) {}
        void addToScene(Shader &shader, int i);
    };
}

#endif