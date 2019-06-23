#ifndef CG_EYE_LIGHT_HPP
#define CG_EYE_LIGHT_HPP

#include <entities/camera.hpp>
#include "directional_light.hpp"

namespace pr {
    struct EyeLight {
        unsigned width, height;
        unsigned depthMapTexture;
        EyeLight();
        void addToScene( Shader &shader, unsigned texUnit );
        void generateShadows( Shader &shader, unsigned int depthMapFrameBuffer, Camera& camera );
    };
}

#endif //CG_EYE_LIGHT_HPP
