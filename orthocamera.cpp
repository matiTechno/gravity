#include <orthocamera.hh>

OrthoCamera::OrthoCamera(float posX, float posY, float rangeX, float rangeY, const glm::vec4& viewport):
    zoom(1.f),
    viewport(viewport),
    position(posX, posY),
    range(rangeX, rangeY)
{}

OrthoCamera::OrthoCamera(const glm::vec2& position, const glm::vec2& range, const glm::vec4& viewport):
    zoom(1.f),
    viewport(viewport),
    position(position),
    range(range)
{}

void OrthoCamera::zoom_to_center(float factor)
{
    zoom *= factor;
    glm::vec2 toMiddle(range / 2.f);
    range *= 1.f / factor;
    position += toMiddle - range / 2.f;
}

void OrthoCamera::zoom_to_mouse(float factor, const glm::vec2& mousePosition)
{
    zoom *= factor;
    glm::vec2 mouse_world_position = get_mouse_world_position(mousePosition);
    range *= 1.f / factor;
    glm::vec2 new_mouse_world_position = get_mouse_world_position(mousePosition);
    position += mouse_world_position - new_mouse_world_position;
}
