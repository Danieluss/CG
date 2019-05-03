#ifndef CG_SHADER_HPP
#define CG_SHADER_HPP

#include "shaderprogram.h"

namespace pr {
    class Shader {
            ShaderProgram *sp;
        public:
            Shader();
            ~Shader();
            ShaderProgram* operator -> ();
    };
}

#endif