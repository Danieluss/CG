#include "eye_light.hpp"
#include <string>
#include <entities/camera.hpp>

namespace pr {
    EyeLight::EyeLight() {
        glGenTextures( 1, &depthMapTexture );
        glBindTexture( GL_TEXTURE_2D, depthMapTexture );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER );
        float borderColor[] = {1.0f, 1.0f, 1.0f};
        glTexParameterfv( GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
        this->width = 1024;
        this->height = 1024;
        glTexImage2D( GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT,
                      GL_FLOAT, NULL );
    }

    void EyeLight::addToScene( Shader &shader, unsigned texUnit ) {
        std::string name = "depthMapTexture";
        glActiveTexture( GL_TEXTURE0 + texUnit );
        glBindTexture( GL_TEXTURE_2D, depthMapTexture );
        shader.setUniform( name.c_str(), texUnit );
    }

    void EyeLight::generateShadows( Shader &shader, unsigned int depthMapFrameBuffer, Camera& camera ) {
        glBindFramebuffer( GL_FRAMEBUFFER, depthMapFrameBuffer );
        glFramebufferTexture2D( GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMapTexture, 0 );
        glDrawBuffer( GL_NONE );
        glReadBuffer( GL_NONE );
        glViewport( 0, 0, width, height );
        glClear( GL_DEPTH_BUFFER_BIT );
        shader.use();
        glm::mat4 PV = camera.projection() * camera.view();
        shader.setUniform( "lightMatrix", PV );
    }
}