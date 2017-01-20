#include <vao.hh>

GLuint VAO::boundID = 0;

VAO::VAO()
{
    auto deleter = [](GLuint* ID)
    {
        glDeleteVertexArrays(1, ID);
        delete ID;
    };

    ID = std::unique_ptr<GLuint, decltype(deleter)>(new GLuint, deleter);

    glGenVertexArrays(1, ID.get());
}

void VAO::bind() const
{
    if(*ID != boundID)
    {
        glBindVertexArray(*ID);
        boundID = *ID;
    }
}

BO::BO()
{
    auto deleter = [](GLuint* ID)
    {
        glDeleteBuffers(1, ID);
        delete ID;
    };

    ID = std::unique_ptr<GLuint, decltype(deleter)>(new GLuint, deleter);

    glGenBuffers(1, ID.get());
}

void BO::bind(GLenum target) const
{
    glBindBuffer(target, *ID);
}
