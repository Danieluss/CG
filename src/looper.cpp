#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <event_handling/listener_manager.hpp>
#include <event_handling/observer.hpp>
#include "looper.hpp"

#include "objects/myCube.h"
#include "looper.hpp"

#include<iostream>

using namespace std;

namespace pr {

    void Looper::drawCube( Shader &shader, glm::mat4 M ) {
        int x = 0;
        shader.setUniform( "material.ambient", x );
        shader.setUniform( "material.diffuse", x );
        glm::vec3 spec = glm::vec3( 1.0, 1.0, 1.0 );
        shader.setUniform( "material.specular", spec );
        float shi = 100.0;
        shader.setUniform( "material.shininess", shi );
        shader.setAttrib( "iPos", 4, myCubeVertices );
        shader.setAttrib( "iTexCoord", 2, myCubeTexCoords );
        shader.setAttrib( "iNormal", 4, myCubeNormals );
        glm::mat3 normalMatrix = glm::transpose( glm::inverse( glm::mat3( M )));
        shader.setUniform( "M", M );
        shader.setUniform( "normalMatrix", normalMatrix );
        shader.draw( {"iPos", "iTexCoord", "iNormal"}, GL_TRIANGLES, myCubeVertexCount );
    }

    void Looper::renderScene( Shader &shader ) {
        glm::mat4 M = glm::mat4( 1.0 );
        float angle = 0.0*M_PI/180.0;
        M = glm::rotate( M, angle, glm::vec3( 1, 0, 0 ));
        shader.setUniform( "M", M );
        glm::mat4 M1;

        M1 = glm::translate( M, glm::vec3( 0, 0, 2 ));
        textures[0].activate( 0 );
        drawCube( shader, M1 );

        M1 = glm::translate( M, glm::vec3( -3, 3, 5 ));
        textures[0].activate( 0 );
        drawCube( shader, M1 );

        M1 = glm::scale( M, glm::vec3( 10.0f, 10.0f, 0.2f ));
        textures[1].activate( 0 );
        drawCube( shader, M1 );
    }

    Looper::Looper( Window &window ) : window( window ),
                                       shader( "src/shaders/glsl/v_main.glsl", "src/shaders/glsl/f_main.glsl" ),
                                       shadowShader( "src/shaders/glsl/v_shadow.glsl",
                                                     "src/shaders/glsl/f_shadow.glsl" ) {
        initListeners();
        initScene();
        //frame buffer for shadows
    }

    Looper::~Looper() {
        for( Texture t : textures ) {
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
            mainCamera.moveFront( deltaTime );
        if( glfwGetKey( window, GLFW_KEY_S ) == GLFW_PRESS )
            mainCamera.moveBack( deltaTime );
        if( glfwGetKey( window, GLFW_KEY_A ) == GLFW_PRESS )
            mainCamera.moveLeft( deltaTime );
        if( glfwGetKey( window, GLFW_KEY_D ) == GLFW_PRESS )
            mainCamera.moveRight( deltaTime );
        if( glfwGetKey( window, GLFW_KEY_SPACE ) == GLFW_PRESS )
            mainCamera.moveUp( deltaTime );
        if( glfwGetKey( window, GLFW_KEY_LEFT_SHIFT ) == GLFW_PRESS )
            mainCamera.moveDown( deltaTime );
    }

    void Looper::render() {
        int numberOfDirectionalLights = min(( int ) directionalLights.size(), 4 );

        for( int i = 0; i < numberOfDirectionalLights; i++ ) {
            directionalLights[i].generateShadows( shadowShader, depthMapFrameBuffer, mainCamera.position.val );
            renderScene( shadowShader );
        }
        glBindFramebuffer( GL_FRAMEBUFFER, 0 );
        glViewport( 0, 0, window.width, window.height );
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
        glm::mat4 V = mainCamera.view();
        glm::mat4 P = mainCamera.projection();
        shader.use();
        shader.setUniform( "P", P );
        shader.setUniform( "V", V );
        shader.setUniform( "cameraLocation", mainCamera.position.val );

        shader.setUniform( "numberOfDirectionalLights", numberOfDirectionalLights );
        for( int i = 0; i < directionalLights.size(); i++ ) {
            directionalLights[i].addToScene( shader, i );
        }
        renderScene( shader );
    }

    void Looper::swap() {
        glfwSwapBuffers( window );
    }

    void Looper::initListeners() {
        ListenerManager.addMouseMotionObs( MotionObserver( [ this ]( glm::vec2 pos ) -> void {
            mainCamera.turn( pos );
        } ));
        ListenerManager.addMouseScrollObs( MotionObserver( [ this ]( glm::vec2 scroll ) -> void {
            mainCamera.zoom( scroll[1] );
        } ));
        ListenerManager.addMappedButtonObs( GLFW_KEY_ESCAPE, ButtonObserver( [ this ]( int, int, int ) -> void {
            glfwSetWindowShouldClose( this->window, true );
        } ));
        ListenerManager.addMappedButtonObs(
                GLFW_KEY_E,
                ButtonObserver(
                        build< ButtonTrigger >().action( GLFW_PRESS ).get(),
                        [ this ]( int, int, int ) -> void {
                            mainCamera.accelerate( 4.f/3.f );
                        }
                ));
        ListenerManager.addMappedButtonObs(
                GLFW_KEY_Q,
                ButtonObserver(
                        build< ButtonTrigger >().action( GLFW_PRESS ).get(),
                        [ this ]( int, int, int ) -> void {
                            mainCamera.accelerate( 0.75f );
                        }
                ));
    }

    void Looper::initScene() {
        glGenFramebuffers( 1, &depthMapFrameBuffer );
        textures.push_back( Texture( "res/textures/bricks.png", 1024, 1024, 3 ));
        textures.push_back( Texture( "res/textures/metal.png", 512, 512, 3 ));
        directionalLights.push_back( DirectionalLight( glm::vec3( -10.0, 10.0, 20.0 ), glm::vec3( 0.3, 0.3, 0.3 ),
                                                       glm::vec3( 0.5, 0.5, 0.5 ), glm::vec3( 1.0, 1.0, 1.0 )));
        directionalLights.push_back( DirectionalLight( glm::vec3( 10.0, -10.0, 20.0 ), glm::vec3( 0.3, 0.3, 0.3 ),
                                                       glm::vec3( 0.5, 0.5, 0.5 ), glm::vec3( 1.0, 1.0, 1.0 )));
    }
}