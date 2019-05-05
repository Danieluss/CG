#ifndef CG_TEXTURE_HPP
#define CG_TEXTURE_HPP

#include<string>
#include<GL/glew.h>

namespace pr {

    #define Elem( element ) element
    #define BegEnum( enumName ) typedef enum tag##enumName
    #define EndEnum( enumName ) enumName; \
            char* str##enumName( enum tag##enumName index );

    BegEnum( TexType ) {
        Elem( DIFFUSE ),
        Elem( SPECULAR ),
        Elem( NORMAL ),
        Elem( HEIGHT )
    }
    EndEnum( TexType )


    struct Texture {

        unsigned int id;
        TexType type;

        Texture( std::string filename, int width, int height, int nrChannels );

        void activate( int num );

        void deleteTexture();

    };
};

#endif