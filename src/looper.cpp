#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include"looper.hpp"

#include"myCube.h"

namespace pr {
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
        glm::mat4 V = glm::lookAt(
            glm::vec3(0,-5,0),
            glm::vec3(0,0,0),
            glm::vec3(0,0,1)
        );
        glm::mat4 P = glm::perspective(50.0*M_PI/180.0, 1.77, 1.0, 50.0);
        shader.use();
        shader.setUniform("P", P);
        shader.setUniform("V", V);

        glm::mat4 M = glm::mat4(1.0);
        shader.setUniform("M", M);

        float triangle[]={
            -1,0,0,1, 1,0,0,1, 0,0,1,1
        };
        shader.setAttrib("vertex", 4, myCubeVertices);
        shader.draw({"vertex"}, GL_TRIANGLES, myCubeVertexCount);
    }

    void Looper::swap() {
        glfwSwapBuffers( window );
    }
}