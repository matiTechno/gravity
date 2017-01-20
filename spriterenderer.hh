#ifndef SPRITERENDERER_HH
#define SPRITERENDERER_HH

#define GLEW_NO_GLU
#include <GL/glew.h>
#include <memory>
#include <vao.hh>
class ShaderProgram;
class Sprite;

class SpriteRenderer
{
public:
    SpriteRenderer();
    SpriteRenderer(const SpriteRenderer&) = delete;
    SpriteRenderer& operator=(const SpriteRenderer&) = delete;

    void render(const ShaderProgram& shader, const Sprite& sprite) const;

private:
    BO vbo;
    VAO vao;
};

#endif // SPRITERENDERER_HH
