#include <viewport.hh>
#define GLEW_NO_GLU
#include <GL/glew.h>

glm::vec4 getViewport(float aspect, const sf::Window& window)
{
    glm::vec2 winSize(window.getSize().x, window.getSize().y);
    float winAspect = winSize.x / winSize.y;
    glm::vec4 viewport;

    if(winAspect > aspect)
    {
        viewport.z = winSize.y * aspect;
        viewport.w = winSize.y;
    }
    else
    {
        viewport.z = winSize.x;
        viewport.w = winSize.x / aspect;
    }
    viewport.x = winSize.x / 2.f - viewport.z / 2.f;
    viewport.y = winSize.y / 2.f - viewport.w / 2.f;

    glViewport(GLint(viewport.x),
               GLint(viewport.y),
               GLint(viewport.z), GLint(viewport.w));

    return viewport;
}

void setViewport(const sf::Window& window)
{
    glViewport(0, 0,
               GLint(window.getSize().x),
               GLint(window.getSize().y));
}

void setViewport(const glm::vec4& viewport)
{
    glViewport(GLint(viewport.x),
               GLint(viewport.y),
               GLint(viewport.z), GLint(viewport.w));
}

bool is_in_viewport(const glm::vec2& pos, const glm::vec4& viewport)
{
    if(pos.x >= viewport.x && pos.x <= viewport.x + viewport.z
            && pos.y >= viewport.y && pos.y <= viewport.y + viewport.w)
        return true;
    return false;
}
