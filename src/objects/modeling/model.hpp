#ifndef CG_MODEL_HPP
#define CG_MODEL_HPP

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <glm/glm.hpp>

#define FACE_VERTEX 0
#define FACE_NORMAL 1
#define FACE_UV 2

namespace pr {
    struct Model {

        std::string name;
        std::vector< glm::vec3 > vertices;
        std::vector< glm::vec2 > uvs;
        std::vector< glm::vec3 > normals;
        std::vector< unsigned * > faces;

        static Model fromFile( std::string filename ) {
            Model model;
            model.name = filename.substr( filename.find_last_of( '/' ) + 1, filename.find_last_of( '.' ));
            std::ifstream objFile;
            std::string rowType;
            try {
                objFile.open( filename );
                while( !objFile.eof()) {
                    objFile>>rowType;
                    if( rowType == "v" ) {
                        glm::vec3 vertex;
                        objFile>>vertex[0]>>vertex[1]>>vertex[2];
                        model.vertices.push_back( vertex );
                    } else if( rowType == "vt" ) {
                        glm::vec2 texture;
                        objFile>>texture[0]>>texture[1];
                        model.uvs.push_back( texture );
                    } else if( rowType == "vn" ) {
                        glm::vec3 normal;
                        objFile>>normal[0]>>normal[1]>>normal[2];
                        model.normals.push_back( normal );
                    } else if( rowType == "f" ) {
                        char delimiter;
                        unsigned *tmp = new unsigned[9];
                        for( int i = 0; i < 9; i += 3 ) {
                            objFile>>tmp[i]>>delimiter>>tmp[i + 1]>>delimiter>>tmp[i + 2];
                        }
                        model.faces.push_back( tmp );
                    }
                }

            } catch( ... ) {
                throw "unable to read object from file: " + filename;
            }
            objFile.close();
            return model;
        }

        virtual ~Model() {
            for( int i = 0; i < faces.size(); i++ ) {
                delete[] faces[i];
            }
        }

    };
}

#endif //CG_MODEL_HPP
