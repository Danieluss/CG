#ifndef CG_SHADER_LOADER_HPP
#define CG_SHADER_LOADER_HPP

#include <string>
#include <fstream>
#include <sstream>

class ShaderLoader {

public:

    static std::string loadShader( std::string filename ) {
        std::ifstream shaderFile;
        try {
            shaderFile.open( filename );
            std::stringstream stream;
            stream << shaderFile.rdbuf();
            return stream.str();
        } catch( ... ) {
            throw "unable to read shader from file: " + filename;
        }
        return nullptr;
    }

};

#endif //CG_SHADER_LOADER_HPP
