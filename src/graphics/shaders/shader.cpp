#include"shader.hpp"
#include<string>

namespace pr {
    Shader::Shader( const char *vertexShader, const char *fragmentShader ) {
        std::string pathToShaders = "src/graphics/shaders/glsl/";
        std::string vs, fs;
        vs = pathToShaders + std::string( vertexShader );
        fs = pathToShaders + std::string( fragmentShader );
        sp = new ShaderProgram( vs.c_str(), NULL, fs.c_str()); //path relative to executable
        id = sp->getShaderId();
    }

    Shader::~Shader() {
        delete sp;
    }

    void Shader::use() {
        glUseProgram( id );
    }

    unsigned int Shader::u( const char *name ) {
        return glGetUniformLocation( id, name );
    }

    unsigned int Shader::a( const char *name ) {
        return glGetAttribLocation( id, name );
    }

    void Shader::setUniform( const char *name, bool &value ) {
        glUniform1i( u( name ), value );
    }

    void Shader::setUniform( const char *name, int &value ) {
        glUniform1i( u( name ), value );
    }

    void Shader::setUniform( const char *name, unsigned int &value ) {
        glUniform1ui( u( name ), value );
    }

    void Shader::setUniform( const char *name, float &value ) {
        glUniform1f( u( name ), value );
    }

    void Shader::setUniform( const char *name, glm::vec3 &value ) {
        glUniform3fv( u( name ), 1, glm::value_ptr( value ));
    }

    void Shader::setUniform( const char *name, glm::vec4 &value ) {
        glUniform3fv( u( name ), 1, glm::value_ptr( value ));
    }

    void Shader::setUniform( const char *name, glm::mat3 &value ) {
        glUniformMatrix3fv( u( name ), 1, false, glm::value_ptr( value ));
    }

    void Shader::setUniform( const char *name, glm::mat4 &value ) {
        glUniformMatrix4fv( u( name ), 1, false, glm::value_ptr( value ));
    }

    void Shader::setAttrib( const char *name, int numberOfValues, float *array ) {
        glVertexAttribPointer( a( name ), numberOfValues, GL_FLOAT, false, 0, array );
        attributes.push_back(name);
    }

    void Shader::enableAttribs( std::vector< const char * > &attributes ) {
        for( auto attrib : attributes ) {
            glEnableVertexAttribArray( a( attrib ));
        }
    }

    void Shader::disableAttribs( std::vector< const char * > &attributes ) {
        for( auto attrib : attributes ) {
            glDisableVertexAttribArray( a( attrib ));
        }
        attributes.clear();
    }

    void Shader::draw( GLenum mode, int size ) {
        enableAttribs( attributes );
        glDrawArrays( mode, 0, size );
        disableAttribs( attributes );
    }

    void Shader::draw( GLenum mode, int size, unsigned int *indices ) {
        enableAttribs( attributes );
        glDrawElements( mode, size, GL_UNSIGNED_INT, indices );
        disableAttribs( attributes );
    }
}