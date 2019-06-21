#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <event_handling/listener_manager.hpp>
#include <event_handling/observer.hpp>
#include "looper.hpp"

#include "graphics/modeling/myCube.h"
#include "looper.hpp"

#include<iostream>

using namespace std;

namespace pr {

    void Looper::drawCube( Shader &shader, glm::mat4 M ) {
        int x = 0;
        glm::vec3 empty = glm::vec3( -1, -1, -1 );
        shader.setUniform( "material.vambient", empty );
        shader.setUniform( "material.vdiffuse", empty );
        shader.setUniform( "material.ambient", x );
        shader.setUniform( "material.diffuse", x );
        glm::vec3 spec = glm::vec3( 1.0, 1.0, 1.0 );
        shader.setUniform( "material.vspecular", spec );
        float shi = 100.0;
        shader.setUniform( "material.shininess", shi );
        shader.setAttrib( "iPos", 3, myCubeVertices );
        shader.setAttrib( "iTexCoord", 2, myCubeTexCoords );
        shader.setAttrib( "iNormal", 3, myCubeVertexNormals );
        glm::mat3 normalMatrix = glm::transpose( glm::inverse( glm::mat3( M )));
        shader.setUniform( "M", M );
        shader.setUniform( "normalMatrix", normalMatrix );
        shader.draw( GL_TRIANGLES, myCubeVertexCount );
    }

    void Looper::renderScene( Shader &shader ) {
        glm::mat4 M = glm::mat4( 1.0 );
//        M = glm::scale( M, {100, 100, 100} );
//        shader.setUniform( "M", M );
//        textures["sky_map"].activate( 0 );
//        models["cube"].draw( shader );
//        float angle = 0.0*M_PI/180.0;
//        M = glm::rotate( M, angle, glm::vec3( 1, 0, 0 ));
//        shader.setUniform( "M", M );
        glm::mat4 M1;
//
//        M1 = glm::translate( M, glm::vec3( 0, 0, 0 ));
//        M1 = glm::rotate( M1, static_cast<float>( -30/180.f*M_PI ), glm::vec3( 0, 1, 0 ) );
//        textures["bricks"].activate( 0 );
//        drawCube( shader, M1 );
//
//        for( int i = 0; i < 10; i++ ) {
//            glm::mat4 tr = glm::translate( glm::mat4(1), glm::vec3( 5, 0, 0 ) );
//            glm::mat4 rot = glm::rotate( glm::mat4(1), static_cast<float>( -30.f/180.f*M_PI ), glm::vec3( 0, 1, 0 ));
//            M1 = tr*rot*M1;
//        }
//        for( int i = 0; i < 10; i++ ) {
//            glm::mat4 tr = glm::translate( glm::mat4(1), glm::vec3( 5, 0, 0 ) );
//            glm::mat4 rot = glm::rotate( glm::mat4(1), static_cast<float>( -30.f/180.f*M_PI ), glm::vec3( 0, 1, 0 ));
//            M1 = inverse( tr * rot ) * M1;
//            textures["bricks"].activate( 0 );
//            drawCube( shader, M1 );
//        }
//
        M1 = glm::scale( M, glm::vec3( 10.0f, 10.0f, 0.2f ));
        textures["metal"].activate( 0 );
        drawCube( shader, M1 );

        for( const auto &strEntityPair : entities ) {
            strEntityPair.second.draw( shader );
        }

    }

    Looper::Looper( Window &window ) : window( window ),
                                       shader( "v_main.glsl", "f_main.glsl" ),
                                       shadowShader( "v_shadow.glsl",
                                                     "f_shadow.glsl" ),
                                       skyboxShader( "v_skybox.glsl",
                                                     "f_skybox.glsl" ) {
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
        deltaTime = updateTime - recentTime;
        processInput();
        updateScene();
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
            fflush(stdout);
            framesTime = 0;
            frameCount = 0;
        }
        glfwPollEvents();
        if( glfwGetKey( window, GLFW_KEY_W ) == GLFW_PRESS )
            currentCamera->move( deltaTime, FORWARD );
        if( glfwGetKey( window, GLFW_KEY_S ) == GLFW_PRESS )
            currentCamera->move( deltaTime, BACKWARD );
        if( glfwGetKey( window, GLFW_KEY_A ) == GLFW_PRESS )
            currentCamera->move( deltaTime, LEFT );
        if( glfwGetKey( window, GLFW_KEY_D ) == GLFW_PRESS )
            currentCamera->move( deltaTime, RIGHT );
        if( glfwGetKey( window, GLFW_KEY_SPACE ) == GLFW_PRESS )
            currentCamera->move( deltaTime, UP );
        if( glfwGetKey( window, GLFW_KEY_LEFT_SHIFT ) == GLFW_PRESS )
            currentCamera->move( deltaTime, DOWN );
    }

    void Looper::renderSkybox() {
        glDepthFunc( GL_LEQUAL );
        skyboxShader.setAttrib( "iPos", 3, myCubeVertices );
        textures["sky_map"].activateCube( 0 );
        skyboxShader.draw( GL_TRIANGLES, myCubeVertexCount );
        glDepthFunc( GL_LESS );
    }

    void Looper::render() {
        int numberOfDirectionalLights = min(( int ) directionalLights.size(), 4 );

        for( int i = 0; i < numberOfDirectionalLights; i++ ) {
            directionalLights[i].generateShadows( shadowShader, depthMapFrameBuffer, currentCamera->position.pos );
            renderScene( shadowShader );
        }
        glBindFramebuffer( GL_FRAMEBUFFER, 0 );
        glViewport( 0, 0, window.width, window.height );
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
        glm::mat4 V = currentCamera->view();
        glm::mat4 P = currentCamera->projection();
        shader.use();
        shader.setUniform( "P", P );
        shader.setUniform( "V", V );
        shader.setUniform( "cameraLocation", currentCamera->position.pos );

        shader.setUniform( "numberOfDirectionalLights", numberOfDirectionalLights );
        for( int i = 0; i < directionalLights.size(); i++ ) {
            directionalLights[i].addToScene( shader, i );
        }
        renderScene( shader );
        skyboxShader.use();
        skyboxShader.setUniform( "P", P );
        skyboxShader.setUniform( "V", V );
        glm::mat4 M = glm::rotate( glm::translate( glm::mat4( 1 ), currentCamera->position.pos ), ( float ) -M_PI/2,
                                   glm::vec3( 1, 0, 0 ));
        skyboxShader.setUniform( "M", M );
        renderSkybox();
    }

    void Looper::updateScene() {
        entities["chalice1"].rotateD( deltaTime*100, Z );
        entities["ufo1"].pos = {0, 0, 1 + 0.5*sin( updateTime*2 )};
        entities["ufo1"].rotateD( deltaTime*200, Z );
    }

    void Looper::swap() {
        glfwSwapBuffers( window );
    }

    void Looper::initListeners() {
        ListenerManager.lockCursor( true );
        ListenerManager.onMouseMotion( MotionObserver( [ this ]( glm::vec2 pos ) -> void {
            currentCamera->turn( pos );
        } ));
        ListenerManager.onMouseScroll( MotionObserver( [ this ]( glm::vec2 scroll ) -> void {
            currentCamera->zoom( scroll[1] );
        } ));
        ListenerManager.onButton( GLFW_KEY_ESCAPE, ButtonObserver( [ this ]( int, int, int ) -> void {
            glfwSetWindowShouldClose( this->window, true );
        } ));
        ListenerManager.onButton( GLFW_KEY_TAB, ButtonObserver(
                build< ButtonTrigger >().action( GLFW_PRESS ).get(),
                [ this ]( int, int, int ) -> void {
                    ListenerManager.lockCursor( !ListenerManager.isCursorLocked());
                    currentCamera->locked = !ListenerManager.isCursorLocked();
                } ));
        ListenerManager.onButton(
                GLFW_KEY_E,
                ButtonObserver(
                        build< ButtonTrigger >().action( GLFW_PRESS ).get(),
                        [ this ]( int, int, int ) -> void {
                            currentCamera->accelerate( 1.25f );
                        }
                ));
        ListenerManager.onButton(
                GLFW_KEY_F,
                ButtonObserver(
                        build< ButtonTrigger >().action( GLFW_PRESS ).get(),
                        [ this ]( int, int, int ) -> void {
                            if( !currentCamera->locked ) {
                                if( currentCamera == &ufoCamera ) {
                                    freeCamera.viewOf( ufoCamera );
                                    currentCamera = &freeCamera;
                                } else if( currentCamera == &freeCamera ) {
                                    currentCamera = &ufoCamera;
                                }
                            }
                        }
                ));
        ListenerManager.onButton(
                GLFW_KEY_Q,
                ButtonObserver(
//                        build< ButtonTrigger >().actions( { GLFW_PRESS, GLFW_REPEAT } ).get(),
                        build< ButtonTrigger >().action( GLFW_PRESS ).get(),
                        [ this ]( int, int, int ) -> void {
                            currentCamera->accelerate( 0.8f );
                        }
                ));
    }

    void Looper::initScene() {
        currentCamera->position.pos = glm::vec3( -5, -5, 3 );
        glGenFramebuffers( 1, &depthMapFrameBuffer );
        textures["sky_map"] = Texture::cubeMap( "sky" );
        textures["bricks"] = Texture( "bricks.png" );
        textures["metal"] = Texture( "metal.png" );
        models["ufo"] = Model( "Low_poly_UFO" );
        models["cube"] = Model( "cube" );
//         models["building"] = Model( "Apartment Building_17_obj" );
//         entities.push_back( Entity( models["building"]));
        models["chalice"] = Model( "chalice" );
        models["eight"] = Model( "eight" );
        entities["chalice1"] = ( Entity( models["chalice"] ));
        entities["eight"] = ( Entity( models["eight"] ));
        entities["chalice1"].translate( {0, 0, 6} );
        entities["chalice1"].rotateD( 60, X );
        entities["eight"].translate( {-11, 0, 1} );
        entities["eight"].rotateD( 90, X );
        entities["ufo1"] = Entity( models["ufo"] );
        entities["ufo1"].rotateD( 90, X );
        entities["ufo1"].scale( {0.1, 0.1, 0.1} );
        entities["chalice2"] = Entity( models["chalice"] );
        entities["chalice2"].translate( {2, 0, 0} );
        entities["chalice2"].rotateD( -90, Z );
        entities["ufo2"] = Entity( models["ufo"] );
        entities["ufo2"].rotateD( 90, X );
        entities["ufo2"].scale( {0.1, 0.1, 0.1} );
        entities["ufo2"].translate( {0, 0, -4.5} );
        entities["chalice2"].setParent( entities["chalice1"] );
        entities["ufo2"].setParent( *currentCamera );
        directionalLights.push_back( DirectionalLight( glm::vec3( -10.0, 10.0, 20.0 ), glm::vec3( 0.3, 0.3, 0.3 ),
                                                       glm::vec3( 0.5, 0.5, 0.5 ), glm::vec3( 1.0, 1.0, 1.0 )));
        directionalLights.push_back( DirectionalLight( glm::vec3( 10.0, -10.0, 20.0 ), glm::vec3( 0.3, 0.3, 0.3 ),
                                                       glm::vec3( 0.5, 0.5, 0.5 ), glm::vec3( 1.0, 1.0, 1.0 )));
    }
}