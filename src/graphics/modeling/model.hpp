#ifndef CG_MODEL_HPP
#define CG_MODEL_HPP

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <glm/glm.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <unordered_map>
#include "texture.hpp"
#include "mesh.hpp"

namespace pr {

    template< class T, class U >
    void rgbcp( T &t, U &u ) {
        t.x = u.r;
        t.y = u.g;
        t.z = u.b;
    }

    template< class T, class U >
    void xyzcp( T &t, U &u ) {
        t.x = u.x;
        t.y = u.y;
        t.z = u.z;
    }

    template< class T, class U >
    void xycp( T &t, U &u ) {
        t.x = u.x;
        t.y = u.y;
    }

    static float squareVertices[] = {-0.5f, -0.5f, -0.5f, 0.5f, 0.5f, -0.5f, 0.5f, 0.5f};
    static float squareUVs[] = {0.f, 0.f, 0.f, 1.f, 1.f, 0.f, 1.f, 1.f};

    struct Model {

        std::string name;
        std::vector< Mesh > meshes;

        Model( const std::string &filename );

        Model();

        void draw( Shader &shader );

        static Model fromFile( const std::string &filename );

    private:

        static std::unordered_map< std::string, Texture > texturesLoaded;

        const aiScene *scene;

        void dfsScene( aiNode *node );

        Mesh meshFrom( aiMesh *aMesh );

        void loadTextures( Mesh& mesh, aiMaterial *material, const aiTextureType& type, const TexType& texType );

    };
}


#endif //CG_MODEL_HPP
