#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include"looper.hpp"

#include"myCube.h"

#include<iostream>

using namespace std;

namespace pr {
    void Looper::drawCube(glm::mat4 M) {
        int x = 0;
        shader.setUniform("material.ambient", x);
        shader.setUniform("material.diffuse", x);
        glm::vec3 spec = glm::vec3(1.0, 1.0, 1.0);
        shader.setUniform("material.specular", spec);
        float shi = 100.0;
        shader.setUniform("material.shininess", shi);
        shader.setAttrib("iPos", 4, myCubeVertices);
        shader.setAttrib("iTexCoord", 2, myCubeTexCoords);
        shader.setAttrib("iNormal", 4, myCubeNormals);
        glm::mat3 normalMatrix = glm::transpose(glm::inverse(glm::mat3(M)));
        shader.setUniform("M", M);
        shader.setUniform("normalMatrix", normalMatrix);
        // for(int i=0; i < 3; i++) {
        //     for(int j=0; j < 3; j++) {
        //         cout << normalMatrix[i][j] << " ";
        //     }
        //     cout << endl;
        // }
        shader.draw({"iPos", "iTexCoord", "iNormal"}, GL_TRIANGLES, myCubeVertexCount);
    }
    Looper::Looper( Window& window, Shader &shader) : window( window ), shader(shader) {
        textures.push_back(Texture("bricks.png", 1024, 1024, 3));
        textures.push_back(Texture("metal.png", 512, 512, 3));
        directionalLights.push_back(DirectionalLight(glm::vec3(1.0, 1.0, 2.0), glm::vec3(0.1, 0.1, 0.1), glm::vec3(1.0, 1.0, 1.0), glm::vec3(1.0, 1.0, 1.0)));
        directionalLights.push_back(DirectionalLight(glm::vec3(1.0, -1.0, 2.0), glm::vec3(0.1, 0.1, 0.1), glm::vec3(0.5, 0.5, 0.5), glm::vec3(1.0, 1.0, 1.0)));
    }
    Looper::~Looper() {
        for(Texture t : textures) {
            t.deleteTexture();
        }
    }
    void Looper::loop() {
        processInput();
        render();
        swap();
    }

    void Looper::processInput() {
        glfwPollEvents();
    }

    void Looper::render() {
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glm::vec3 cameraLocation = glm::vec3(-5, -5, 6);
        glm::mat4 V = glm::lookAt(
            cameraLocation,
            glm::vec3(0,0,0),
            glm::vec3(0,0,1)
        );
        glm::mat4 P = glm::perspective(50.0*M_PI/180.0, 1.77, 1.0, 50.0);
        shader.use();
        shader.setUniform("P", P);
        shader.setUniform("V", V);
        shader.setUniform("cameraLocation", cameraLocation);

        int numberOfDirectionalLights = (int)directionalLights.size();
        shader.setUniform("numberOfDirectionalLights", numberOfDirectionalLights);
        for(int i=0; i < directionalLights.size(); i++) {
            directionalLights[i].addToScene(shader, i);
        }

        glm::mat4 M = glm::mat4(1.0);
        float angle = 0.0*M_PI/180.0;
        M = glm::rotate(M, angle, glm::vec3(1,0,0));
        shader.setUniform("M", M);
        glm::mat4 M1;

        M1 = glm::translate(M, glm::vec3(0, 0, 2));
        textures[0].activate(0);
        drawCube(M1);

        M1 = glm::scale(M, glm::vec3(5.0f, 5.0f, 0.2f));
        textures[1].activate(0);
        drawCube(M1);

    }

    void Looper::swap() {
        glfwSwapBuffers( window );
    }
}