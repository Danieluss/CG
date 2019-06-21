#ifndef CG_TEXTURE_HPP
#define CG_TEXTURE_HPP

#include <string>
#include <GL/glew.h>

namespace pr {

    enum TexType {

        DIFFUSE, SPECULAR, AMBIENT, NORMAL

    };

    struct Texture {

        unsigned int id;
        TexType type;

        Texture();

        static Texture cubeMap( std::string filename );

        Texture( std::string filename );

        void activate( int num );

        void activateCube( int num );

        void deleteTexture();

        bool operator==( const Texture& texture );

    };

}

namespace std {
    template<>
    struct hash< pr::Texture > {
        size_t operator()( const pr::Texture &obj ) const {
            return hash<unsigned>()( obj.id );
        }
    };
}

#endif