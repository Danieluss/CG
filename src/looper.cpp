#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <event_handling/listener_manager.hpp>
#include <event_handling/observer.hpp>
#include"looper.hpp"

#include"models/myCube.h"
#include "looper.hpp"

namespace pr {

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
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
        glm::mat4 V = camera.view();
        glm::mat4 P = camera.projection();
        shader.use();
        shader.setUniform( "P", P );
        shader.setUniform( "V", V );

        glm::mat4 M = glm::mat4( 1.0 );
        shader.setUniform( "M", M );

        shader.setAttrib( "vertex", 4, myCubeVertices );
        shader.draw( {"vertex"}, GL_TRIANGLES, myCubeVertexCount );
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