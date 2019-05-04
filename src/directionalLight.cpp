#include"directionalLight.hpp"
#include<string>
#include<iostream>

namespace pr {
    void DirectionalLight::addToScene(Shader &shader, int i) {
        std::string name = "directionalLights[" + std::to_string(i) + "].";
        std::string u;
        u = name + "direction";
        //std::cout << u << std::endl;
        shader.setUniform(u.c_str(), direction);
        u = name + "ambient";
        shader.setUniform(u.c_str(), ambient);
        u = name + "diffuse";
        shader.setUniform(u.c_str(), diffuse);
        u = name + "specular";
        shader.setUniform(u.c_str(), specular);
    }
}