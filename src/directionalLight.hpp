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
        glm::mat4 lightMatrix;
        unsigned int depthMapTexture;
    public:
        DirectionalLight(glm::vec3 direction, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular);
        void addToScene(Shader &shader, int i);
        void generateShadows(Shader &shader, unsigned int depthMapFrameBuffer, glm::vec3 cameraLocation);
    };
}

#endif