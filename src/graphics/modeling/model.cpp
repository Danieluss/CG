#include "model.hpp"
#include "texture.hpp"

#include<iostream>
using namespace std;

std::unordered_map< std::string, pr::Texture > pr::Model::texturesLoaded = std::unordered_map< std::string, pr::Texture >();

int i = 0;

pr::Model::Model( const std::string &filename ) {
    name = filename;
    Assimp::Importer importer;
    scene = importer.ReadFile( "res/models/" + filename + ".obj", aiProcess_Triangulate | aiProcess_CalcTangentSpace );
    if( !scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode ) {
        throw ( "assimp error while loading: " + filename + " description: " + importer.GetErrorString());
    }
    dfsScene( scene->mRootNode );
    importer.FreeScene();
    scene = nullptr;
}

void pr::Model::draw( pr::Shader &shader ) {
    for( int i = 0; i < meshes.size(); i++ ) {
        meshes[i].draw( shader );
    }
}

pr::Model pr::Model::fromFile( const std::string &filename ) {
    Model model( filename );
    return model;
}


void pr::Model::dfsScene( aiNode *node ) {
    for( int i = 0; i < node->mNumMeshes; i++ ) {
        meshes.push_back( meshFrom( scene->mMeshes[node->mMeshes[i]] ));
    }
    for( int i = 0; i < node->mNumChildren; i++ ) {
        dfsScene( node->mChildren[i] );
    }
}

pr::Mesh pr::Model::meshFrom( aiMesh *aMesh ) {
    Mesh mesh;
    for( int i = 0; i < aMesh->mNumVertices; i++ ) {
        Vertex vertex;
        xyzcp( vertex.position, aMesh->mVertices[i] );
        if( aMesh->HasNormals() ) {
            xyzcp( vertex.normal, aMesh->mNormals[i] );
        }
        if( aMesh->HasTangentsAndBitangents() ) {
            xyzcp( vertex.tangent, aMesh->mTangents[i] );
            xyzcp( vertex.bitangent, aMesh->mBitangents[i] );
        }
        if( aMesh->mTextureCoords[0] )
            xycp( vertex.uv, aMesh->mTextureCoords[0][i] );
        mesh.vertices.push_back( vertex );
    }
    for( int i = 0; i < aMesh->mNumFaces; i++ ) {
        aiFace &face = aMesh->mFaces[i];
        for( int j = 0; j < face.mNumIndices; j++ ) {
            mesh.indices.push_back( face.mIndices[j] );
        }
    }
    aiMaterial *material = scene->mMaterials[aMesh->mMaterialIndex];
    aiColor4D tmp;
    aiGetMaterialFloat( material, AI_MATKEY_SHININESS, &mesh.shininess );
    aiGetMaterialColor( material, AI_MATKEY_COLOR_AMBIENT, &tmp );
    rgbcp( mesh.ambient, tmp );
    aiGetMaterialColor( material, AI_MATKEY_COLOR_SPECULAR, &tmp );
    rgbcp( mesh.specular, tmp );
    aiGetMaterialColor( material, AI_MATKEY_COLOR_DIFFUSE, &tmp );
    rgbcp( mesh.diffuse, tmp );
    loadTextures( mesh, material, aiTextureType_DIFFUSE, DIFFUSE );
    loadTextures( mesh, material, aiTextureType_SPECULAR, SPECULAR );
    loadTextures( mesh, material, aiTextureType_HEIGHT, NORMAL );
    loadTextures( mesh, material, aiTextureType_AMBIENT, AMBIENT );
    mesh.updateArrays();
    return mesh;
}

void
pr::Model::loadTextures( pr::Mesh &mesh, aiMaterial *material, const aiTextureType &type, const pr::TexType &texType ) {
    for( int i = 0; i < material->GetTextureCount( type ); i++ ) {
        aiString string;
        material->GetTexture( type, i, &string );
        std::string filename = std::string( string.C_Str() );
        if( texturesLoaded.find( filename ) == texturesLoaded.end() ) {
            Texture* texture;
            try {
                texture = new Texture( name + "/" + filename );
            } catch (...) {
                texture = new Texture( filename );
            }
            texturesLoaded[ filename ] = *texture;
            texture->type = texType;
            mesh.textures.push_back( *texture );
        } else {
            Texture texture = texturesLoaded[filename];
            texture.type = texType;
            mesh.textures.push_back(texture);
        }
    }
}

pr::Model::Model() {

}
