#include"directionalLight.hpp"
#include<string>
#include<iostream>

using namespace std;

#define SHADOW_HEIGHT 1024
#define SHADOW_WIDTH 1024

namespace pr {
    DirectionalLight::DirectionalLight(glm::vec3 direction, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular) : direction(direction), ambient(ambient), diffuse(diffuse), specular(specular) {
        glGenTextures(1, &depthMapTexture);
        glBindTexture(GL_TEXTURE_2D, depthMapTexture);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); 
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    }
    void DirectionalLight::addToScene(Shader &shader, int i) {
        std::string name = "directionalLights[" + std::to_string(i) + "].";
        std::string u;
        u = name + "direction";
        shader.setUniform(u.c_str(), direction);
        u = name + "ambient";
        shader.setUniform(u.c_str(), ambient);
        u = name + "diffuse";
        shader.setUniform(u.c_str(), diffuse);
        u = name + "specular";
        shader.setUniform(u.c_str(), specular);
        u = name + "depthMapTexture";
        int num = 15-i;
        glActiveTexture(GL_TEXTURE0+num);
        glBindTexture(GL_TEXTURE_2D, depthMapTexture);
        shader.setUniform(u.c_str(), num);
        u = name + "lightMatrix";
        shader.setUniform(u.c_str(), lightMatrix);
    }
    void DirectionalLight::generateShadows(Shader &shader, unsigned int depthMapFrameBuffer, glm::vec3 cameraLocation) {
        glBindFramebuffer(GL_FRAMEBUFFER, depthMapFrameBuffer);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMapTexture, 0);
        glDrawBuffer(GL_NONE);
        glReadBuffer(GL_NONE);
        glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
        glClear(GL_DEPTH_BUFFER_BIT);
        float d = glm::length(direction);
        glm::mat4 P = glm::ortho(-10.0f, 20.0f, -20.0f, 20.0f, d-10.0f, d+10.0f);
        glm::mat4 V;
        if(abs(direction[0]) < 0.01 && abs(direction[1]) < 0.01) {
            V = glm::lookAt(cameraLocation+direction, cameraLocation, glm::vec3(1,0,0));
        } else {
            V = glm::lookAt(cameraLocation+direction, cameraLocation, glm::vec3(0,0,1));
        }
        lightMatrix = P*V;
        shader.use();
        shader.setUniform("lightMatrix", lightMatrix);
    }
}