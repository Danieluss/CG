#ifndef CG_SHADER_HPP
#define CG_SHADER_HPP

#include<glm/glm.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<vector>
#include "shaderprogram.h"

namespace pr {
    class Shader {
            ShaderProgram *sp;
            unsigned int shaderId;
            unsigned int u(const char *name);
            unsigned int a(const char *name);
            void enableAttribs(std::vector<const char *> &attributes);
            void disableAttribs(std::vector<const char *> &attributes);
        public:
            Shader(const char *vertexShader, const char *fragmentShader);
            ~Shader();
            void use();
            void setUniform(const char *name, bool &value);
            void setUniform(const char *name, int &value);
            void setUniform(const char *name, unsigned int &value);
            void setUniform(const char *name, float &value);
            void setUniform(const char *name, glm::vec3 &value);
            void setUniform(const char *name, glm::vec4 &value);
            void setUniform(const char *name, glm::mat3 &value);
            void setUniform(const char *name, glm::mat4 &value);
            void setAttrib(const char *name, int numberOfValues, float *array);
            void draw(std::vector<const char *> attributes, GLenum mode, int size);
            void draw(std::vector<const char *> attributes, GLenum mode, int size, unsigned int *indices);
    };
}

#endif