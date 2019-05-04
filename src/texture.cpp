#include"texture.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include<iostream>

namespace pr {
    Texture::Texture(std::string filename, int width, int height, int nrChannels) {
        glGenTextures(1, &textureId);
        glBindTexture(GL_TEXTURE_2D, textureId);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        unsigned char *data = stbi_load(filename.c_str(), &width, &height, &nrChannels, 0);
        if (data) {
            //glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            //glGenerateMipmap(GL_TEXTURE_2D);
            glTexImage2D(GL_TEXTURE_2D, 0, 3, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        }
        else {
            throw("Error while loading texture");
        }
        stbi_image_free(data);
    }
    void Texture::deleteTexture() {
        glDeleteTextures(1, &textureId);
    }
    void Texture::activate(int num) {
        glActiveTexture(GL_TEXTURE0+num);
        glBindTexture(GL_TEXTURE_2D, textureId);
    }
}