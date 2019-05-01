#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include"looper.hpp"

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
        shader->use();
        glUniformMatrix4fv(shader->u("P"), 1, false, glm::value_ptr(P));
        glUniformMatrix4fv(shader->u("V"), 1, false, glm::value_ptr(V));

        glm::mat4 M = glm::mat4(1.0);
        glUniformMatrix4fv(shader->u("M"), 1, false, glm::value_ptr(M));

        glEnableVertexAttribArray(shader->a("vertex"));
        float triangle[]={
            -1,0,0,1, 1,0,0,1, 0,0,1,1
        };
        glVertexAttribPointer(shader->a("vertex"), 4, GL_FLOAT, false, 0, triangle);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glDisableVertexAttribArray(shader->a("vertex"));
    }

    void Looper::swap() {
        glfwSwapBuffers( window );
    }
}