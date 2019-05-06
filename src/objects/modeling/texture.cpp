#include"texture.hpp"

#define STB_IMAGE_IMPLEMENTATION

#include "stb_image.h"
#include "texture.hpp"


#include<iostream>

namespace pr {

    Texture::Texture( std::string filename ) {
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
            glTexImage2D( GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data );
            glGenerateMipmap( GL_TEXTURE_2D );

            //Optional parameters - need additional research
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
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

}