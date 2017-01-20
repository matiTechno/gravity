#ifndef VAO_HH
#define VAO_HH

#define GLEW_NO_GLU
#include <GL/glew.h>
#include <memory>

class VAO
{
public:
    VAO();
    void bind() const;

private:
    static GLuint boundID;
    std::unique_ptr<GLuint, std::function<void(GLuint*)>> ID;
};

class BO
{
public:
    BO();
    void bind(GLenum target) const;

private:
    std::unique_ptr<GLuint, std::function<void(GLuint*)>> ID;
};

#endif // VAO_HH
