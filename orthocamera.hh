#ifndef ORTHOCAMERA_HH
#define ORTHOCAMERA_HH

#include <glm/gtc/matrix_transform.hpp>

class OrthoCamera
{
public:
    OrthoCamera(float left, float right, float bottom, float top, const glm::vec4& viewport);

    OrthoCamera(const glm::vec2& position, const glm::vec2& range, const glm::vec4& viewport);

    float getZoom() const;

    const glm::vec2& getPosition() const;

    const glm::vec2& getRange() const;

    void movePosition(const glm::vec2& vec);

    void zoom_to_center(float factor);

    void zoom_to_mouse(float factor, const glm::vec2& mousePosition);

    glm::mat4 getProjetion() const;

    glm::vec2 get_mouse_world_position(const glm::vec2& mousePosition) const;

    float getAspect() const;

private:
    float zoom;
    const glm::vec4& viewport;
    glm::vec2 position;
    glm::vec2 range;
};

inline float OrthoCamera::getZoom() const
{
    return zoom;
}

inline const glm::vec2& OrthoCamera::getPosition() const
{
    return position;
}

inline const glm::vec2& OrthoCamera::getRange() const
{
    return range;
}

inline void OrthoCamera::movePosition(const glm::vec2& vec)
{
    position += vec;
}

inline glm::mat4 OrthoCamera::getProjetion() const
{
    return glm::ortho(position.x, position.x + range.x, position.y + range.y, position.y);
}

inline glm::vec2 OrthoCamera::get_mouse_world_position(const glm::vec2& mousePosition) const
{
    return (mousePosition - glm::vec2(viewport.x, viewport.y)) *
            range / glm::vec2(viewport.z, viewport.w) + position;
}

inline float OrthoCamera::getAspect() const
{
    return range.x / range.y;
}

#endif // ORTHOCAMERA_HH
