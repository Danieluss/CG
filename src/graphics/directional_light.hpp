#ifndef CG_DIRLIGHT_HPP
#define CG_DIRLIGHT_HPP

#include<glm/glm.hpp>
#include"shaders/shader.hpp"
#include"modeling/model.hpp"
#include"entities/basic_entities.hpp"

namespace pr {
    struct DirectionalLight {
        glm::vec3 direction;
        glm::vec3 ambient;
        glm::vec3 diffuse;
        glm::vec3 specular;
        glm::mat4 lightMatrix;
        unsigned int depthMapTexture;
        Entity sunEntity;
        DirectionalLight(glm::vec3 direction, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular);
        void addToScene(Shader &shader, int i);
        void generateShadows(Shader &shader, unsigned int depthMapFrameBuffer, glm::vec3 cameraLocation, glm::vec3 cameraDirection);
        void addObject(Model &model);
        void draw(Shader &shader, Positionable position);
    };
}

#endif