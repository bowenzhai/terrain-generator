#ifndef __TEXTURE_H__
#define __TEXTURE_H__

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#define STB_IMAGE_IMPLEMENTATION
#include <includes/stb_image.h>

#include <iostream>

// helper class from LearnOpenGL
class Texture {
private:
    unsigned int id;

public:
    void gen() {
        glGenTextures(1, &id);
    }

    void bind() {
        // bind so any subsequent texture commands will configure the currently bound texture
        glBindTexture(GL_TEXTURE_2D, id);
    }

    void setOptions() {
        // set the texture wrapping/filtering options (on the currently bound texture object)
        // s, t, r are the coordinates for textures
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }

    void load(const char *path) {
        int width, height, nrChannels;
        // load the texture
        unsigned char *data = stbi_load(path, &width, &height, &nrChannels, 0);
        if (data) {
            // generate(target, mipmap level, format, w, h, 0, src_format, src_datatype, img_data)
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        else {
            std::cout << "Failed to load texture" << std::endl;
        }
        stbi_image_free(data);
    }
};

#endif