#ifndef SPRITE_HH
#define SPRITE_HH

#include <glm/vec2.hpp>
#include <glm/vec4.hpp>
class Texture;

// rotation uses radians
// point of rotation = top-left corner
// rgb <0 - 255> alpha <0 - 1>
class Sprite
{
public:
    Sprite():
        rotation(0.f),
        color(255.f, 255.f, 255.f, 1.f)
    {}

    const Texture* texture;
    glm::vec2 position;
    glm::vec2 size;
    float rotation;
    glm::uvec4 texCoords;
    glm::vec4 color;
};

#endif // SPRITE_HH
