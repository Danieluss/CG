#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <event_handling/listener_manager.hpp>
#include <event_handling/observer.hpp>
#include"looper.hpp"

#include"models/myCube.h"
#include "looper.hpp"

#include<iostream>

using namespace std;

namespace pr {

    void Looper::drawCube(Shader &shader, glm::mat4 M) {
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
        shader.draw({"iPos", "iTexCoord", "iNormal"}, GL_TRIANGLES, myCubeVertexCount);
    }

    void Looper::renderScene(Shader &shader) {
        glm::mat4 M = glm::mat4(1.0);
        float angle = 0.0*M_PI/180.0;
        M = glm::rotate(M, angle, glm::vec3(1,0,0));
        shader.setUniform("M", M);
        glm::mat4 M1;

        M1 = glm::translate(M, glm::vec3(0, 0, 2));
        textures[0].activate(0);
        drawCube(shader, M1);

        M1 = glm::translate(M, glm::vec3(-3, 3, 5));
        textures[0].activate(0);
        drawCube(shader, M1);

        M1 = glm::scale(M, glm::vec3(10.0f, 10.0f, 0.2f));
        textures[1].activate(0);
        drawCube(shader, M1);
    }

    Looper::Looper( Window& window) : window( window ), shader("src/v_main.glsl", "src/f_main.glsl"), shadowShader("src/v_shadow.glsl", "src/f_shadow.glsl") {
        //frame buffer for shadows
        glGenFramebuffers(1, &depthMapFrameBuffer);
        textures.push_back(Texture("bricks.png", 1024, 1024, 3));
        textures.push_back(Texture("metal.png", 512, 512, 3));
        directionalLights.push_back(DirectionalLight(glm::vec3(-10.0, 10.0, 20.0), glm::vec3(0.3, 0.3, 0.3), glm::vec3(0.5, 0.5, 0.5), glm::vec3(1.0, 1.0, 1.0)));
        directionalLights.push_back(DirectionalLight(glm::vec3(10.0, -10.0, 20.0), glm::vec3(0.3, 0.3, 0.3), glm::vec3(0.5, 0.5, 0.5), glm::vec3(1.0, 1.0, 1.0)));
    }
    Looper::~Looper() {
        for(Texture t : textures) {
            t.deleteTexture();
        }
    }
    void Looper::loop() {
        updateTime = glfwGetTime();
        processInput();
        render();
        swap();
        recentTime = updateTime;
    }

    void Looper::processInput() {
        double deltaTime = updateTime - recentTime;
        glfwPollEvents();
        if( glfwGetKey( window, GLFW_KEY_W ) == GLFW_PRESS )
            camera.moveFront( deltaTime );
        if( glfwGetKey( window, GLFW_KEY_S ) == GLFW_PRESS )
            camera.moveBack( deltaTime );
        if( glfwGetKey( window, GLFW_KEY_A ) == GLFW_PRESS )
            camera.moveLeft( deltaTime );
        if( glfwGetKey( window, GLFW_KEY_D ) == GLFW_PRESS )
            camera.moveRight( deltaTime );
        if( glfwGetKey( window, GLFW_KEY_SPACE ) == GLFW_PRESS )
            camera.moveUp( deltaTime );
        if( glfwGetKey( window, GLFW_KEY_LEFT_SHIFT ) == GLFW_PRESS )
            camera.moveDown( deltaTime );
    }

    void Looper::render() {
        glm::vec3 cameraLocation = glm::vec3(-5, -5, 6);
        int numberOfDirectionalLights = min((int)directionalLights.size(), 4);

        for(int i=0; i < numberOfDirectionalLights; i++) {
            directionalLights[i].generateShadows(shadowShader, depthMapFrameBuffer, cameraLocation);
            renderScene(shadowShader);
        }
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glViewport(0, 0, window.width, window.height);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
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

        shader.setUniform("numberOfDirectionalLights", numberOfDirectionalLights);
        for(int i=0; i < directionalLights.size(); i++) {
            directionalLights[i].addToScene(shader, i);
        }
        renderScene(shader);
    }

    void Looper::swap() {
        glfwSwapBuffers( window );
    }

    void Looper::init() {
        ListenerManager.addMouseMotionObs( MotionObserver( [this]( glm::vec2 pos ) -> void{
            camera.turn( pos );
        } ) );
        ListenerManager.addMouseScrollObs( MotionObserver( [this]( glm::vec2 scroll ) -> void{
            camera.zoom( scroll[1] );
        } ) );
        ListenerManager.addMappedButtonObs( GLFW_KEY_ESCAPE, ButtonObserver( [this]( int, int, int ) -> void{
            glfwSetWindowShouldClose( window, true );
        } ) );
        ListenerManager.addMappedButtonObs(
                GLFW_KEY_E,
                ButtonObserver(
                        build<ButtonTrigger>().action( GLFW_PRESS ).get(),
                        [this]( int, int, int ) -> void{
                            camera.accelerate( 2.f ); }
                ) );
        ListenerManager.addMappedButtonObs(
                GLFW_KEY_Q,
                ButtonObserver(
                        build<ButtonTrigger>().action( GLFW_PRESS ).get(),
                        [this]( int, int, int ) -> void{
                            camera.accelerate( 0.5f ); }
                        ) );
    }
}