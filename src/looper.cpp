#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "event_handling/listener_manager.hpp"
#include "event_handling/observer.hpp"
#include "looper.hpp"

#include "graphics/modeling/myCube.h"
#include "looper.hpp"

#include<iostream>
#include "entities/basic_entities.hpp"

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
        textures["bricks"].activate( 0 );
        drawCube( shader, M1 );

        M1 = glm::translate( M, glm::vec3( -3, 3, 5 ));
        textures["bricks"].activate( 0 );
        drawCube( shader, M1 );

        M1 = glm::scale( M, glm::vec3( 10.0f, 10.0f, 0.2f ));
        textures["metal"].activate( 0 );
        drawCube( shader, M1 );

        for( auto entity : entities ) {
            entity.draw( shader );
        }
    }

    Looper::Looper( Window &window ) : window( window ),
                                       shader( "v_main.glsl", "f_main.glsl" ),
                                       shadowShader( "v_shadow.glsl",
                                                     "f_shadow.glsl" ) {
        initListeners();
        initScene();
    }

    Looper::~Looper() {
        for( auto pair : textures ) {
            pair.second.deleteTexture();
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
        frameCount++;
        framesTime += deltaTime;
        if( framesTime >= fpsRefresh ) {
            std::cout<<"\rFPS: "<<frameCount/framesTime;
            fflush( stdout );
            framesTime = 0;
            frameCount = 0;
        }
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
        ListenerManager.onMouseMotion( MotionObserver( [ this ]( glm::vec2 pos ) -> void {
            mainCamera.turn( pos );
        } ));
        ListenerManager.onMouseScroll( MotionObserver( [ this ]( glm::vec2 scroll ) -> void {
            mainCamera.zoom( scroll[1] );
        } ));
        ListenerManager.onButton( GLFW_KEY_ESCAPE, ButtonObserver( [ this ]( int, int, int ) -> void {
            glfwSetWindowShouldClose( this->window, true );
        } ));
        ListenerManager.onButton(
                GLFW_KEY_E,
                ButtonObserver(
                        build< ButtonTrigger >().action( GLFW_PRESS ).get(),
                        [ this ]( int, int, int ) -> void {
                            mainCamera.accelerate( 1.25f );
                        }
                ));
        ListenerManager.onButton(
                GLFW_KEY_Q,
                ButtonObserver(
//                        build< ButtonTrigger >().actions( { GLFW_PRESS, GLFW_REPEAT } ).get(),
                        build< ButtonTrigger >().action( GLFW_PRESS ).get(),
                        [ this ]( int, int, int ) -> void {
                            mainCamera.accelerate( 0.8f );
                        }
                ));
    }

    void Looper::initScene() {
        mainCamera.position.val = glm::vec3(-5,-5,3);
        glGenFramebuffers( 1, &depthMapFrameBuffer );
        textures["bricks"] = Texture( "bricks.png" );
        textures["metal"] = Texture( "metal.png" );
        models["chalice"] = Model( "chalice.obj" );
        entities.push_back( Entity( models[ "chalice" ] ) );
        directionalLights.push_back( DirectionalLight( glm::vec3( -10.0, 10.0, 20.0 ), glm::vec3( 0.3, 0.3, 0.3 ),
                                                       glm::vec3( 0.5, 0.5, 0.5 ), glm::vec3( 1.0, 1.0, 1.0 )));
        directionalLights.push_back( DirectionalLight( glm::vec3( 10.0, -10.0, 20.0 ), glm::vec3( 0.3, 0.3, 0.3 ),
                                                       glm::vec3( 0.5, 0.5, 0.5 ), glm::vec3( 1.0, 1.0, 1.0 )));
    }
}