#include"texture.hpp"

#define STB_IMAGE_IMPLEMENTATION

#include "stb_image.h"
#include "texture.hpp"


#include<vector>
#include<iostream>

namespace pr {

    inline bool approx( double x, double y ) {
        return abs( x - y ) < 0.0001;
    }

    inline int closestPower( int x ) {
        return ( int ) ( pow( 2, ceil( log2( x ))) + 0.1 );
    }

    Texture::Texture( std::string filename ) {
        std::cout<<filename<<std::endl;
        filename = "res/textures/" + filename;
        glGenTextures( 1, &id );
        glBindTexture( GL_TEXTURE_2D, id );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
        int width, height, noChannels;
        unsigned char *data = stbi_load( filename.c_str(), &width, &height, &noChannels, 0 );
        if( data ) {
            //glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            //glGenerateMipmap(GL_TEXTURE_2D);
            GLenum format;
            if( noChannels == 1 )
                format = GL_RED;
            else if( noChannels == 3 )
                format = GL_RGB;
            else if( noChannels == 4 )
                format = GL_RGBA;

            glBindTexture( GL_TEXTURE_2D, id );
            //TODO
            //loading res/textures/Miami_2525/f1q.jpg causes 0xC0000005 here
            // width - 1 height - 1 works, but gives wrong results later
            std::cout<<filename<<std::endl;
            if( approx( log2( width ), round( log2( width ) ) ) ) {
                std::cout<<"A"<<std::endl;
                glTexImage2D( GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data );
            } else {
                std::cout<<"B"<<std::endl;
                glTexImage2D( GL_TEXTURE_2D, 0, format, width - 1, height - 1, 0, format, GL_UNSIGNED_BYTE, data );
            }
            //TODO
            glGenerateMipmap( GL_TEXTURE_2D );

            //Optional parameters - need additional research
            glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
            glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
            glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
            glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
        } else {
            throw ( "Error while loading texture" );
        }
        stbi_image_free( data );
    }


    void Texture::deleteTexture() {
        glDeleteTextures( 1, &id );
    }

    void Texture::activate( int num ) {
        glActiveTexture( GL_TEXTURE0 + num );
        glBindTexture( GL_TEXTURE_2D, id );
    }

    bool Texture::operator==( const Texture &texture ) {
        return id == texture.id;
    }

    Texture::Texture() {

    }

    Texture Texture::cubeMap( std::string filename ) {
        Texture texture;
        std::string path = "res/textures/" + filename + "/";
        glGenTextures( 1, &texture.id );
        glBindTexture( GL_TEXTURE_CUBE_MAP, texture.id );
        int width, height, noChannels;
        std::vector< std::string > faces = {"right.png",
                                            "left.png",
                                            "top.png",
                                            "bottom.png",
                                            "front.png",
                                            "back.png"};
        for( GLuint i = 0; i < 6; i++ ) {
            unsigned char *data = stbi_load(( path + faces[i] ).c_str(), &width, &height, &noChannels, 0 );
            if( data ) {
                glTexImage2D( GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                              0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
                );
                stbi_image_free( data );
            } else {
                std::cout<<"Cubemap texture failed to load at path: "<<( filename + "/" + faces[i] )<<std::endl;
                stbi_image_free( data );
            }
        }
        glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
        glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
        glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
        glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
        glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE );
        return texture;
    }

    void Texture::activateCube( int num ) {
        glActiveTexture( GL_TEXTURE0 + num );
        glBindTexture( GL_TEXTURE_CUBE_MAP, id );
    }

}