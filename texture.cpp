#include <texture.hh>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

GLuint Texture::boundID = 0;

Texture::Texture(const std::string& filename)
{
    unsigned char* data = stbi_load(filename.c_str(), &size.x, &size.y, nullptr, 4);
    if(!data)
        throw std::runtime_error("stbi_load failed: " + filename);

    auto deleter = [](GLuint* ID)
    {
        glDeleteTextures(1, ID);
        delete ID;
    };

    ID = std::unique_ptr<GLuint, decltype(deleter)>(new GLuint, deleter);

    glGenTextures(1, ID.get());
    bind();

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, size.x, size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    stbi_image_free(data);
}

Texture::Texture(GLint internalFormat, GLsizei width, GLsizei height, GLenum format,
                 GLenum type):
    size(width, height)
{
    auto deleter = [](GLuint* ID)
    {
        glDeleteTextures(1, ID);
        delete ID;
    };

    ID = std::unique_ptr<GLuint, decltype(deleter)>(new GLuint, deleter);

    glGenTextures(1, ID.get());
    bind();

    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, type, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}

void Texture::bind(GLuint unit) const
{
    if(*ID != boundID)
    {
        glActiveTexture(GL_TEXTURE0 + unit);
        glBindTexture(GL_TEXTURE_2D, *ID);
        boundID = *ID;
    }
}

GLuint Texture::getID() const
{
    return *ID;
}

TexMan::TexMan(const std::string& path):
    path(path)
{}

Texture& TexMan::get(const std::string& filename)
{
    auto tex = textures.find(filename);
    if(tex == textures.end())
    {
        if(!path.empty())
            return textures.emplace(filename, path + '/' + filename).first->second;
        else
            return textures.emplace(filename, filename).first->second;
    }
    return tex->second;
}
