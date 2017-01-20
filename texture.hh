#ifndef TEXTURE_HH
#define TEXTURE_HH

#include <glm/vec2.hpp>
#define GLEW_NO_GLU
#include <GL/glew.h>
#include <string>
#include <memory>
#include <unordered_map>

class Texture
{
public:
    Texture(const std::string& filename);
    Texture(GLint internalFormat, GLsizei width, GLsizei height, GLenum format,
            GLenum type);

    const glm::ivec2& getSize() const;
    void bind(GLuint unit = 0) const;
    GLuint getID() const;

private:
    std::unique_ptr<GLuint, std::function<void(GLuint*)>> ID;
    glm::ivec2 size;
    static GLuint boundID;
};

inline const glm::ivec2& Texture::getSize() const
{
    return size;
}

class TexMan
{
public:
    TexMan(const std::string& path);
    TexMan() = default;
    TexMan(const TexMan&) = delete;
    TexMan& operator=(const TexMan&) = delete;

    Texture& get(const std::string& filename);

private:
    std::unordered_map<std::string, Texture> textures;
    std::string path;
};

#endif // TEXTURE_HH
