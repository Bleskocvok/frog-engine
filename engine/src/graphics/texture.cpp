#include "texture.hpp"

// include the image loading library
#include <stb_image.h>

#include <memory>           // std:unique_ptr
#include <cmath>            // std::log2


namespace
{


struct Deleter
{
    void operator()(unsigned char* data) { stbi_image_free(data); }
};

using image = std::unique_ptr<unsigned char[], Deleter>;


GLuint load_file(const std::string& filename)
{
    GLuint texture = 0;
    glCreateTextures(GL_TEXTURE_2D, 1, &texture);

    int width = 0,
        height = 0,
        channels = 0,
        desired_channels = 4;   // rgba

    image img
    {
        stbi_load(filename.c_str(),
                  &width,
                  &height,
                  &channels,
                  desired_channels)
    };

    glTextureStorage2D(texture,                     // the texture id
                       1,                           // num of LOD levels
                       GL_RGBA8,                    // format
                       width, height);              // image size

    glTextureSubImage2D(texture,                    // the texture id
                        0,                          // default LOD
                        0, 0,                       // offset
                        width, height,              // size
                        GL_RGBA,                    // format
                        GL_UNSIGNED_BYTE,           // data type
                        img.get());                 // pointer to image data

    // set texture filtering
    glTextureParameteri(texture,
                        GL_TEXTURE_MIN_FILTER,      // minifying filter
                        GL_LINEAR);                 // we want linear
    glTextureParameteri(texture,
                        GL_TEXTURE_MAG_FILTER,      // magnifying filter
                        GL_LINEAR);                 // again, linear

    // make it repeat beyond the edges
    glTextureParameteri(texture, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTextureParameteri(texture, GL_TEXTURE_WRAP_T, GL_REPEAT);

    return texture;
}


} // namespace


frog::gl::texture::texture(const std::string& filename)
    : _index(load_file(filename)) {}


frog::gl::texture::~texture()
{
    glDeleteTextures(1, &_index);
}


void frog::gl::texture::bind(GLuint idx)
{
    glBindTextureUnit(idx, _index);
}


