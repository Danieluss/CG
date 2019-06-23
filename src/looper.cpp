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
        glm::vec3 tmp = glm::vec3( 1, 1, 1 );
        shader.setUniform( "material.vnormal", tmp );
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
                                                     "f_skybox.glsl" ),
                                       collisionShader( "v_collision.glsl",
                                                        "f_collision.glsl" ),
                                       particleShader( "v_particle.glsl",
                                                    "f_particle.glsl")
                                                                        {
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

    bool Looper::detectCollision( MoveDir dir ) {
        return false;
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
        vector< pair< int, MoveDir>> moves = {
                {GLFW_KEY_W,            FORWARD},
                {GLFW_KEY_S,            BACKWARD},
                {GLFW_KEY_A,            LEFT},
                {GLFW_KEY_D,            RIGHT},
                {GLFW_KEY_X,            STOP},
                {GLFW_KEY_SPACE,        UP},
                {GLFW_KEY_LEFT_CONTROL, DOWN}
        };
        if( currentCamera == &ufoCamera || currentCamera == &thirdPersonCamera ) {
            if( glfwGetKey( window, GLFW_KEY_LEFT_SHIFT ) == GLFW_PRESS ) {
                currentCamera->speed = 1.f;
            } else {
                currentCamera->speed = 0.4f;
            }
        }

        for( auto &m : moves ) {
            if( glfwGetKey( window, m.first ) == GLFW_PRESS ) {
                Inertiable previousPosition = currentCamera->position;
                currentCamera->move( deltaTime, m.second );
                if( detectCollision( m.second )) {
                    currentCamera->position = previousPosition;

                }
            }
        }
//         if( glfwGetKey( window, GLFW_KEY_W ) == GLFW_PRESS )
//             currentCamera->move( deltaTime, FORWARD );
        // if( glfwGetKey( window, GLFW_KEY_S ) == GLFW_PRESS )
        //     currentCamera->move( deltaTime, BACKWARD );
        // if( glfwGetKey( window, GLFW_KEY_A ) == GLFW_PRESS )
        //     currentCamera->move( deltaTime, LEFT );
        // if( glfwGetKey( window, GLFW_KEY_D ) == GLFW_PRESS )
        //     currentCamera->move( deltaTime, RIGHT );
        // if( glfwGetKey( window, GLFW_KEY_SPACE ) == GLFW_PRESS )
        //     currentCamera->move( deltaTime, UP );
        // if( glfwGetKey( window, GLFW_KEY_LEFT_SHIFT ) == GLFW_PRESS )
        //     currentCamera->move( deltaTime, DOWN );
    }

    void Looper::renderParticles() {
//        glDisable(GL_DEPTH_TEST);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glDepthMask( false );
        glm::mat4 V = currentCamera->view();
        particleShader.setUniform( "camNear", currentCamera->minDist );
        particleShader.setUniform( "camFar", currentCamera->maxDist );
        for( auto& pair : particles ) {
            auto& particle = pair.second;
            glm::mat4 M = glm::translate( glm::mat4(1), particle.position );
//            glm::mat3 tSubV = glm::transpose( glm::mat3( V ) );
            for( int i = 0; i < 3; i++ ) {
                for( int j = 0; j < 3; j++ ) {
                    M[i][j] = V[j][i];
                }
            }
            M = glm::rotate( M, glm::radians( particle.rot2d ), {0, 0, -1} );
            M = glm::scale( M, particle.scale );
            glm::mat4 VM = V * M;
            particleShader.setUniform( "VM", VM );
            particleShader.setUniform( "alpha", particle.alpha );
            particleShader.setAttrib( "pos", 2, squareVertices );
            particleShader.setAttrib( "iTexCoord", 2, squareUVs );
            unsigned i = 0;// REEEEEEE
            particle.texture->activate( i );
            //
            std::string str = "sprite";
//            particleShader.setUniform( str.c_str(), i );
            particleShader.draw( GL_TRIANGLE_STRIP, 4 );
        }
        glDisable(GL_BLEND);
        glDepthMask( true );
//        glEnable(GL_DEPTH_TEST);
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
        eyeLight.generateShadows( shadowShader, depthMapFrameBuffer, *currentCamera );
        renderScene( shadowShader );

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
        glm::mat4 M = glm::rotate( glm::translate( glm::mat4( 1 ), currentCamera == &thirdPersonCamera ?
                                                                   thirdPersonCamera.position.pos -
                                                                   thirdPersonCamera.dir*thirdPersonCamera.distance :
                                                                   currentCamera->position.pos ),
                                   ( float ) -M_PI/2,
                                   glm::vec3( 1, 0, 0 ));
        skyboxShader.setUniform( "M", M );
        renderSkybox();
        particleShader.use();
        eyeLight.addToScene( particleShader, 1 );
        particleShader.setUniform( "P", P );
        renderParticles();
    }

    void Looper::updateScene() {
        entities["chalice1"].rotateD( deltaTime*100, Z );
//        entities["ufo1"].pos = {0, 0, 1 + 0.5*sin( updateTime*2 )};
        entities["ufo1"].rotateD( deltaTime*200, Z );
//        entities["ufo1"].pos = {0, 0, 0.25*sin( updateTime*2 )};
        entities["ufo1"].update( deltaTime );
        ufoCamera.position.update( deltaTime );
        thirdPersonCamera.position.update( deltaTime );
        entities["player_ufo"].rotateD( deltaTime*50, Z );
        entities["player_ufo"].pos = {0, 0, -4.5 + 0.2*sin( updateTime*2 )};
        vector<std::string> rmParticles;
        for( auto pair : particles ) {
            if( particles[pair.first].update( deltaTime )) {
                rmParticles.push_back( pair.first );
            }
        }
        for( std::string string : rmParticles ) {
            particles.erase( particles.find( string ));
        }
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
                                    previousCamera = &ufoCamera;
                                } else if( currentCamera == &thirdPersonCamera ) {
                                    freeCamera.viewOf( thirdPersonCamera );
                                    currentCamera = &freeCamera;
                                    previousCamera = &thirdPersonCamera;
                                } else if( currentCamera == &freeCamera ) {
                                    currentCamera = previousCamera;
                                }
                            }
                        }
                ));
        ListenerManager.onButton(
                GLFW_KEY_T,
                ButtonObserver(
                        build< ButtonTrigger >().action( GLFW_PRESS ).get(),
                        [ this ]( int, int, int ) -> void {
                            if( !currentCamera->locked ) {
                                if( currentCamera == &ufoCamera ) {
                                    thirdPersonCamera.viewOf( ufoCamera );
                                    currentCamera = &thirdPersonCamera;
                                    entities["player_ufo"].setParent( thirdPersonCamera );
                                } else if( currentCamera == &thirdPersonCamera ) {
                                    ufoCamera.viewOf( thirdPersonCamera );
                                    currentCamera = &ufoCamera;
                                    entities["player_ufo"].setParent( ufoCamera );
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

    void Looper::initCollisions() {
        vector< pair< MoveDir, unsigned int > > data = {
                {FORWARD,  0},
                {BACKWARD, 0},
                {LEFT,     0},
                {RIGHT,    0},
                {UP,       0},
                {DOWN,     0}
        };
        // for(auto &d : data) {
        //     collisionTextures[d.first] = d.second;
        // }
        // for(auto &c : collisionTextures) {
        //     glGenTextures(1, &c.second);
        //     glBindTexture(GL_TEXTURE_2D, c.second);
        //     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        //     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        //     glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, COLLISION_TEX_SIZE, COLLISION_TEX_SIZE, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
        //     collisionShader.use();

        //     entities["player_ufo"].draw(collisionShader);
        // }
    }

    void Looper::initScene() {
        textures["boom"] = Texture( "greenboom.png" );
        particles["pp"].texture = &textures["boom"];
        particles["pp"].fade = neverFade();
//        particles["pp"].fade = sinTransition( 10, 2 );
//        particles["pp"].fade = sinTransition( 10, 2 );
        particles["pp"].scale = {1, 1, 1};
        particles["pp"].translation = constTranslation( {0, 0, 1} );
        particles["pp1"].texture = &textures["boom"];
        particles["pp1"].translation = constTranslation( {3, 3, 3} );
        particles["pp1"].fade = sinTransition( 10, 2 );
        particles["pp1"].scale = {10, 10, 10};
        currentCamera->position.pos = glm::vec3( -5, -5, 3 );
        glGenFramebuffers( 1, &depthMapFrameBuffer );
        textures["sky_map"] = Texture::cubeMap( "sky" );
        textures["bricks"] = Texture( "bricks.png" );
        textures["metal"] = Texture( "metal.png" );
        models["ufo"] = Model( "Low_poly_UFO" );
//        models["city"] = Model( "Miami_2525" );
//        models["building1"] = Model( "Miami_2525" );
        models["cube"] = Model( "cube" );
//         entities["building1"] = Entity( models["building1"]);
//         entities["building1"].rotateD( 90, X );
//         entities["building1"].scale( { 1, 1, 1 } );
        models["chalice"] = Model( "chalice" );
        models["eight"] = Model( "eight" );
        entities["chalice1"] = ( Entity( models["chalice"] ));
        entities["eight"] = ( Entity( models["eight"] ));
        entities["chalice1"].translate( {0, 0, 6} );
        entities["chalice1"].rotateD( 60, X );
        entities["eight"].translate( {-11, 0, 1} );
        entities["eight"].rotateD( 90, X );
        entities["ufo1"] = Entity( models["ufo"] );
        entities["ufo1"].addInertia( {0, 0, 1}, 0.1, 1 );
        entities["ufo1"].rotateD( 90, X );
        entities["ufo1"].scale( {0.1, 0.1, 0.1} );
        entities["chalice2"] = Entity( models["chalice"] );
        entities["chalice2"].translate( {2, 0, 0} );
        entities["chalice2"].rotateD( -90, Z );
        entities["player_ufo"] = Entity( models["ufo"] );
        entities["player_ufo"].rotateD( 90, X );
        entities["player_ufo"].scale( {0.1, 0.1, 0.1} );
        entities["player_ufo"].translate( {0, 0, -4.5} );
        entities["chalice2"].setParent( entities["chalice1"] );
        entities["player_ufo"].setParent( *currentCamera );
        directionalLights.push_back( DirectionalLight( glm::vec3( -10.0, 10.0, 20.0 ), glm::vec3( 0.3, 0.3, 0.3 ),
                                                       glm::vec3( 0.5, 0.5, 0.5 ), glm::vec3( 1.0, 1.0, 1.0 )));
        directionalLights.push_back( DirectionalLight( glm::vec3( 10.0, -10.0, 20.0 ), glm::vec3( 0.3, 0.3, 0.3 ),
                                                       glm::vec3( 0.5, 0.5, 0.5 ), glm::vec3( 1.0, 1.0, 1.0 )));
        recentTime = glfwGetTime();
    }
}
