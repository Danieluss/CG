#include"shader.hpp"

namespace pr {
    Shader::Shader() {
        sp=new ShaderProgram("src/v_test.glsl",NULL,"src/f_test.glsl"); //path relative to executable
    }
    Shader::~Shader() {
        delete sp;
    }
    ShaderProgram* Shader::operator-> () {
        return sp;
    }
}