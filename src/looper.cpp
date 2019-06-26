#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <event_handling/listener_manager.hpp>
#include <event_handling/observer.hpp>
#include <time_effect.hpp>
#include "looper.hpp"
#include "good_random.hpp"
#include <set>
#include "graphics/modeling/chunk.hpp"
#include "graphics/modeling/myCube.h"
#include "looper.hpp"
#include "utilities.hpp"
#include <iostream>

#define rn GoodRandom::rng.get()

using namespace std;

namespace pr {

    inline glm::vec3 rngVec() {
        glm::vec3 vec;
        for( int i = 0; i < 2; i++ ) {
            vec[i] = rn;
        }
        return vec;
    }

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

    void Looper::renderScene( Shader &shader, bool playerUfoVisible ) {
        glm::mat4 M = glm::mat4( 1.0 );
        glm::mat4 M1;
        M1 = glm::scale( M, glm::vec3( 10.0f, 10.0f, 0.2f ));
        textures["metal"].activate( 0 );
        drawCube( shader, M1 );

        for( const auto &strEntityPair : entities ) {
            if( playerUfoVisible == false && strEntityPair.first == "player_ufo" ) {
                continue;
            }
            strEntityPair.second.draw( shader );
        }

        renderChunks(shader);
    }

    void Looper::renderChunks( Shader &shader ) {
        glm::vec3 pos = currentCamera->position.pos;
        int x = floor(pos.x/Chunk::chunkSize);
        int y = floor(pos.y/Chunk::chunkSize);
        glm::vec3 dir = currentCamera->dir;
        std::set<std::pair<int, int>> skipped;
        skipped.insert({-sgn(dir.x), -sgn(dir.y)});

        for(int i=-2; i <= 2; i++) {
            for(int j=-2; j <= 2; j++) {
                if(abs(i) > 0 && abs(j) > 0 && skipped.count({sgn(i), sgn(j)})) {
                    continue;
                }
                int nx = x+i;
                int ny = y+j;
                if(chunks.count({nx, ny}) == 0) {
                    chunks[{nx, ny}] = Chunk(models, 1, nx, ny);
                    chunks[{nx, ny}].setParents();
                }
                if(abs(i) > 1 || abs(j) > 1) {
                    chunks[{nx, ny}].drawBasic(shader);
                } else {
                    chunks[{nx, ny}].draw(shader);
                }
            }
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
                                                       "f_particle.glsl" ),
                                        sunShader("v_sun.glsl", "f_sun.glsl")               
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

    void Looper::processInput() {
        double deltaTime = updateTime - recentTime;
        frameCount++;
        framesTime += deltaTime;
        if( framesTime >= fpsRefresh ) {
            std::cout<<"\rFPS: "<<frameCount/framesTime<<" dist: "<<maxv;
            fflush(stdout);
            framesTime = 0;
            frameCount = 0;
            maxv = 0;
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
                currentCamera->move( deltaTime, m.second );
            }
        }
    }

    void Looper::renderParticles() {
        glDisable( GL_DEPTH_TEST );
        glEnable( GL_BLEND );
        glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
        glDepthMask( false );
        glm::mat4 V = currentCamera->view();
        float width = window.width;
        float height = window.height;
        particleShader.setUniform( "camNear", currentCamera->minDist );
        particleShader.setUniform( "camFar", currentCamera->maxDist );
        particleShader.setUniform( "width", width );
        particleShader.setUniform( "height", height );
        for( auto &pair : particles ) {
            auto &particle = pair.second;
            glm::mat4 M = glm::translate( glm::mat4( 1 ), particle.position );
//            glm::mat3 tSubV = glm::transpose( glm::mat3( V ) );
            for( int i = 0; i < 3; i++ ) {
                for( int j = 0; j < 3; j++ ) {
                    M[i][j] = V[j][i];
                }
            }
            M = glm::rotate( M, glm::radians( particle.rot2d ), {0, 0, -1} );
            M = glm::scale( M, particle.scale );
            glm::mat4 VM = V*M;
            particleShader.setUniform( "VM", VM );
            particleShader.setUniform( "alpha", particle.alpha );
            particleShader.setUniform( "scale", particle.scale.x );
            particleShader.setAttrib( "pos", 2, squareVertices );
            particleShader.setAttrib( "iTexCoord", 2, squareUVs );
            int i = 0;// REEEEEEE
            particle.texture->activate( i );
            //
            std::string str = "sprite";
//            particleShader.setUniform( str.c_str(), i );
            particleShader.draw( GL_TRIANGLE_STRIP, 4 );
        }
        glDisable( GL_BLEND );
        glDepthMask( true );
        glEnable( GL_DEPTH_TEST );
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
            directionalLights[i].generateShadows( shadowShader, depthMapFrameBuffer, currentCamera->position.pos, currentCamera->dir);
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
        glEnable(GL_MULTISAMPLE);
        renderScene( shader );
        glDisable(GL_MULTISAMPLE);
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
        sunShader.use();
        sunShader.setUniform("P", P);
        sunShader.setUniform("V", V);
        for(int i=0; i < directionalLights.size(); i++) {
            directionalLights[i].draw(sunShader, currentCamera->position);
        }
        particleShader.use();
        eyeLight.addToScene( particleShader, 1 );
        particleShader.setUniform( "P", P );
        renderParticles();
    }

    void Looper::updateScene() {
//        static double noParticleTime;
//        noParticleTime += deltaTime;
//        if( noParticleTime > 2.1 ) {
//            noParticleTime -= 2.1;
//            for( int i = 0; i < 10; i++ ) {
//                std::string id = std::to_string( i );
//                particles[id].texture = &textures["boom"];
//                particles[id].fade = linearTransition( 2, 1, 0 );
//                //        particles["pp"].fade = sinTransition( 10, 2 );
//                //        particles["pp"].fade = sinTransition( 10, 2 );
//                particles[id].scale = {3, 3, 3};
//                particles[id].translation = linearTranslation( {5 + i, 0, 1}, {0, 0, 1} );
//            }
//        }

        std::vector< std::string > rmEffects;

        for( auto& pair : effects ) {
            if( pair.second.play( updateTime ) ) {
                rmEffects.push_back( pair.first );
            }
        }

        for( auto& string : rmEffects ) {
            effects.erase( string );
        }

        entities["chalice1"].rotateD( deltaTime*100, Z );
//        entities["ufo1"].pos = {0, 0, 1 + 0.5*sin( updateTime*2 )};
        entities["ufo1"].rotateD( deltaTime*200, Z );
//        entities["ufo1"].pos = {0, 0, 0.25*sin( updateTime*2 )};
        entities["ufo1"].update( deltaTime );

        Inertiable previousPosition = ufoCamera.position;
        if( currentCamera == &thirdPersonCamera ) {
            previousPosition = thirdPersonCamera.position;
        }
        ufoCamera.position.update( deltaTime );
        thirdPersonCamera.position.update( deltaTime );
        entities["player_ufo"].rotateD( deltaTime*50, Z );
        entities["player_ufo"].pos = {0, 0, -4.5 + 0.06*sin( updateTime*2 )};
        glm::vec3 dm;
        if( detectCollision( dm )) {
            if( !exploding && getUFOCamera()->position.inertia > 1 ) {
                exploding = true;
                collisionEffect();
            }
            if( currentCamera == &thirdPersonCamera ) {
                thirdPersonCamera.position.pos += dm*3.f;
                thirdPersonCamera.position.inertiaDir = glm::reflect( thirdPersonCamera.position.inertiaDir,
                                                                      glm::normalize( dm ));
                thirdPersonCamera.position.inertia *= 0.6;
            } else {
                ufoCamera.position.pos += dm*3.f;
                ufoCamera.position.inertiaDir = glm::reflect( ufoCamera.position.inertiaDir, glm::normalize( dm ));
                thirdPersonCamera.position.inertia *= 0.6;
            }
        }

        vector< std::string > rmParticles;
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

#define deb( A ) cout << A[0] << " " << A[1] << " " << A[2] << "\n"

    std::vector< float > Looper::getDepthVector( glm::vec3 dir, bool ufoVisible ) {
        glBindFramebuffer( GL_FRAMEBUFFER, collisionMapFrameBuffer );
        glFramebufferTexture2D( GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, collisionTexture, 0 );
        glDrawBuffer( GL_NONE );
        glReadBuffer( GL_NONE );
        glViewport( 0, 0, COLLISION_TEX_SIZE, COLLISION_TEX_SIZE );
        glClear( GL_DEPTH_BUFFER_BIT );

        // glBindFramebuffer( GL_FRAMEBUFFER, 0 );
        // glViewport( 0, 0, window.width, window.height );
        // glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

        collisionShader.use();
        float a = COLLISION_VIEW_SIZE;
        glm::mat4 P = glm::ortho( -a, a, -a, a, 0.f, 2*a );
        glm::vec3 eye, center, nose;
        UFOCamera camera;
        if( currentCamera == &thirdPersonCamera ) {
            camera.viewOf( thirdPersonCamera );
        } else if( currentCamera == &ufoCamera ) {
            camera.viewOf( ufoCamera );
        } else {
            if( previousCamera == &thirdPersonCamera ) {
                camera.viewOf( thirdPersonCamera );
            } else {
                camera.viewOf( ufoCamera );
            }
        }
        center = camera.position.pos;
        // camera.move(1.0, dir);
        // camera.position.update(1.0);
        // eye = camera.position.pos;
        glm::vec3 v = 2*a*glm::normalize( dir );
        center[2] -= 2;
        eye = center + v;
        if( ufoVisible == false ) {
            std::swap( eye, center );
        }
        nose = glm::vec3( 0, 0, 1 );
        if( dir == glm::vec3( 0, 0, 1 )) {
            nose = glm::vec3( -1, 0, 0 );
        } else if( dir == glm::vec3( 0, 0, -1 )) {
            nose = glm::vec3( 1, 0, 0 );
        }
        glm::mat4 V = glm::lookAt( eye, center, nose );
        glm::mat4 PV = P*V;
        collisionShader.setUniform( "P", P );
        collisionShader.setUniform( "V", V );
        if( ufoVisible ) {
            entities["player_ufo"].draw( collisionShader );
        } else {
            renderScene( collisionShader, false );
        }
        // glfwSwapBuffers( window );
        vector< float > tex;
        tex.resize( COLLISION_TEX_SIZE*COLLISION_TEX_SIZE );
        glBindTexture( GL_TEXTURE_2D, collisionTexture );
        glGetTexImage( GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, GL_FLOAT, tex.data());
        return tex;
    }

    bool Looper::detectCollision( glm::vec3 &translation ) {
        vector< glm::vec3 > data = {
                glm::vec3( 1, 0, 0 ), glm::vec3( -1, 0, 0 ), glm::vec3( 0, 1, 0 ), glm::vec3( 0, -1, 0 ),
                glm::vec3( 0, 0, 1 ), glm::vec3( 0, 0, -1 )
        };
        translation = glm::vec3( 0, 0, 0 );
        bool res = false;
        for( auto d : data ) {
            vector< float > a = getDepthVector( d, true );
            vector< float > b = getDepthVector( d, false );
            float maxValue = 0.0;
            for( int i = 0; i < COLLISION_TEX_SIZE; i++ ) {
                for( int j = 0; j < COLLISION_TEX_SIZE; j++ ) {
                    float ufo = 1.0 - a[COLLISION_TEX_SIZE*( i + 1 ) - 1 - j];
                    float world = 1.0 - b[COLLISION_TEX_SIZE*i + j];
                    if( ufo < 0.001 ) {
                        continue;
                    }
                    maxValue = max( maxValue, ufo + world );
                }
            }
            if( maxValue > 0.95 ) {
                translation -= d*( maxValue - 0.95f );
                res = true;
            }
            maxv = max( maxv, maxValue );
        }
        return res;
    }

    void Looper::initCollisions() {
        glGenTextures( 1, &collisionTexture );
        glBindTexture( GL_TEXTURE_2D, collisionTexture );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
        glTexImage2D( GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, COLLISION_TEX_SIZE, COLLISION_TEX_SIZE, 0,
                      GL_DEPTH_COMPONENT, GL_FLOAT, NULL );
        glGenFramebuffers( 1, &collisionMapFrameBuffer );
    }

    void Looper::initScene() {
        textures["greenboom"] = Texture( "greenboom.png" );
        textures["blueboom"] = Texture( "blueboom.png" );
        textures["boomboom"] = Texture( "boomboom.png" );
        textures["sparks"] = Texture( "sparks.png" );
        currentCamera->position.pos = glm::vec3( -5, -5, 4 );
        glGenFramebuffers( 1, &depthMapFrameBuffer );
        textures["sky_map"] = Texture::cubeMap( "sky" );
        textures["bricks"] = Texture( "bricks.png" );
        textures["metal"] = Texture( "metal.png" );
        models["ufo"] = Model( "Low_poly_UFO" );
        //    models["city"] = Model( "Miami_2525" );
        //    models["building1"] = Model( "Miami_2525" );
        models["cube"] = Model( "cube" );
        // entities["building1"] = Entity( models["building1"]);
        // entities["building1"].rotateD( 90, X );
        // entities["building1"].scale( { 1, 1, 1 } );
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

        models["grass"] = Model("grass");
        models["grass"].multiplyVertices({72.0, 72.0, 0.5}, {10.0, 10.0});

        models["road1"] = Model("road");
        models["road1"].multiplyVertices({90.0, 6.0, 0.5}, {15.0, 1.0});
        models["road2"] = Model("road");
        models["road2"].multiplyVertices({6.0, 90.0, 0.5}, {1.0, 15.0});

        models["pavement1"] = Model("pavement");
        models["pavement1"].multiplyVertices({78.0, 3.0, 0.5}, {26.0, 1.0});
        models["pavement2"] = Model("pavement");
        models["pavement2"].multiplyVertices({3.0, 78.0, 0.5}, {1.0, 26.0});

        models["building1"] = Model("Residential Buildings 001");
        models["building2"] = Model("Residential Buildings 002");
        models["building3"] = Model("Residential Buildings 003");
        models["building4"] = Model("Residential Buildings 004");

        models["sun"] = Model("sun");
        models["sun"].multiplyVertices({20.0, 20.0, 20.0}, {1.0, 1.0});

        directionalLights.push_back( DirectionalLight( glm::vec3( 10.0, 10.0, 10.0 ), glm::vec3( 0.2, 0.2, 0.2 ),
                                                       glm::vec3( 0.4, 0.4, 0.4 ), glm::vec3( 1.0, 1.0, 1.0 )));
        directionalLights.push_back( DirectionalLight( glm::vec3( 10.0, -10.0, 10.0 ), glm::vec3( 0.2, 0.2, 0.2 ),
                                                       glm::vec3( 0.4, 0.4, 0.4 ), glm::vec3( 1.0, 1.0, 1.0 )));
        directionalLights[0].addObject(models["sun"]);
        directionalLights[1].addObject(models["sun"]);
        recentTime = glfwGetTime();
        initCollisions();
//        glm::vec3 x = glm::vec3(0.f, 1.f, 0.f);
//        touchEffect(x);
    }

    void Looper::collisionEffect() {
        std::vector< std::pair<double, std::function<void()>> > events;
        events.push_back( {
            0.1, [ & ]() -> void {
                touchEffect();
            }
        } );
        events.push_back( {
            0.5, [ & ]() -> void {
                sparkingEffect();
            }
        } );
        events.push_back( {
            2, [ & ]() -> void {
                smokeEffect();
            }
        } );
        events.push_back( {
            4, [ & ]() -> void {
                boomEffect();
            }
        } );
        effects[std::to_string( effectId )].setEvents( events );
        effects[std::to_string( effectId++ )].relative( updateTime );
    }

    void Looper::sparkingEffect() {
        std::vector< std::pair<double, std::function<void()>> > events;
        for( int i = 0; i < 20; i++ ) {
            events.push_back( {
                i * 0.5, [ & ]() -> void {
                    sparksParticle();
                }
            } );
        }
        effects[std::to_string( effectId )].setEvents( events );
        effects[std::to_string( effectId++ )].relative( updateTime );
    }

    void Looper::boomEffect() {
        std::vector< std::pair<double, std::function<void()>> > events;
        for( int i = 0; i < 100; i++ ) {
            events.push_back( {
                i * 0.06, [ & ]() -> void {
                    boomParticle();
                }
            } );
        }
        events.push_back( {
            0, [ & ]() -> void {
                exploding = false;
            }
        } );
        effects[std::to_string( effectId )].setEvents( events );
        effects[std::to_string( effectId++ )].relative( updateTime );
    }

    void Looper::touchEffect() {
        std::vector< std::pair<double, std::function<void()>> > events;
        events.push_back( {
            0, [ & ]() -> void {
                electricalParticle();
                electricalParticle();
            }
        } );
        effects[std::to_string( effectId )].setEvents( events );
        effects[std::to_string( effectId++ )].relative( updateTime );
    }

    void Looper::smokeEffect() {
        std::vector< std::pair<double, std::function<void()>> > events;
        for( int i = 0; i < 300; i++ ) {
            events.push_back( {
                i * 0.1, [ & ]() -> void {
                    smokeParticle();
                    smokeParticle();
                }
            } );
        }
        effects[std::to_string( effectId )].setEvents( events );
        effects[std::to_string( effectId++ )].relative( updateTime );
    }

    Camera *Looper::getUFOCamera() {
        if( currentCamera == &ufoCamera )
            return &ufoCamera;
        else if( currentCamera == &thirdPersonCamera )
            return &thirdPersonCamera;
        else
            return previousCamera;
    }

    void Looper::smokeParticle() {
        glm::vec3 pos = getUFOCamera()->position.pos;
        std::string id = std::to_string( particleId++ );
        particles[id].texture = &textures["greenboom"];
        particles[id].scaling = acceleratedTranslation( glm::vec3{1, 1, 1}, {1.5, 1.5, 1.5}, {-0.1, -0.1, -0.1} );
        glm::vec3 tpos = (pos + glm::vec3{rn*5 - 2.5, rn*5 - 2.5, -2});
        glm::vec3 vel = {0, 0, 2};
        glm::vec3 acc = {0, 0, -0.1};
        particles[id].translation = acceleratedTranslation( tpos, vel, acc);
//                    particles[id].translation = acceleratedTranslation( pos, rngVec()*0.2f - 0.1f, rngVec()*0.2f - 0.1f);
        particles[id].rotation = linearTransition( 2, ( rn - 0.5 )*180, ( rn - 0.5 )*180 );
        particles[id].fade = cosTransition( 2, 0.3 );
    }

    void Looper::electricalParticle() {
        glm::vec3 pos = getUFOCamera()->position.pos;
        std::string id = std::to_string( particleId++ );
        particles[id].texture = &textures["blueboom"];
        particles[id].scaling = acceleratedTranslation( glm::vec3{1, 1, 1}, {1, 1, 1}, {500, 500, 500} );
        particles[id].translation = constTranslation( pos + glm::vec3{0, 0, -2} );
//                    particles[id].translation = acceleratedTranslation( pos, rngVec()*0.2f - 0.1f, rngVec()*0.2f - 0.1f);
        particles[id].rotation = linearTransition( 0.5, ( rn - 0.5 )*180, ( rn - 0.5 )*180 );
        particles[id].fade = linearTransition( 0.5, 0.5, 0 );
    }

    void Looper::sparksParticle() {
        glm::vec3 pos = rngVec() * glm::vec3{ 4.f, 4.f, -0.5f} - glm::vec3{2, 2, 0};
        pos += getUFOCamera()->position.pos;
        auto scaling = linearTranslation( {1, 1, 1}, {5, 5, 5} );
        auto translation = acceleratedTranslation( pos + glm::vec3{ 4*rn - 2, 4*rn - 2, -1.5 }, {2 * rn - 1, 2 * rn - 1, 3}, {0, 0, -10} );
        auto fade = cosTransition( 3, 0.8 );
        auto rotation = constTransition( 3, 0 );
        std::string id = std::to_string( particleId++ );
        particles[id].texture = &textures["sparks"];
        particles[id].scaling = scaling;
        particles[id].translation = translation;
        particles[id].rotation = rotation;
        particles[id].fade = fade;
    }

    void Looper::boomParticle() {
        glm::vec3 pos = getUFOCamera()->position.pos;
        std::string id = std::to_string( particleId++ );
        particles[id].texture = &textures["boomboom"];
        particles[id].scaling = acceleratedTranslation( glm::vec3{1, 1, 1}  * (float) rn * 2.f  + 0.2f, {20, 20, 20}, {-100, -100, -100} );
        glm::vec3 tpos = (glm::vec3{ 5 * rn - 2.5, 5 * rn - 2.5, rn * -1.f - 1.5} + pos);
        glm::vec3 vel = (rngVec() * -0.05f + 0.025f);
        glm::vec3 acc = (rngVec() * -0.05f + 0.025f);
        particles[id].translation = acceleratedTranslation( tpos, vel, acc);
//                    particles[id].translation = acceleratedTranslation( pos, rngVec()*0.2f - 0.1f, rngVec()*0.2f - 0.1f);
        particles[id].rotation = linearTransition( 0.2, ( rn - 0.5 )*5, ( rn - 0.5 )*5 );
        particles[id].fade = sinTransition( 0.2, 0.3 );
    }

}
