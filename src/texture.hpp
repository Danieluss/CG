#ifndef CG_TEXTURE_HPP
#define CG_TEXTURE_HPP

#include<string>
#include<GL/glew.h>

namespace pr {
    class Texture {
    public:
        unsigned int textureId;
    public:
        Texture(std::string filename, int widht, int height, int nrChannels);
        void activate(int num);
        void deleteTexture();
    };
};

#endif