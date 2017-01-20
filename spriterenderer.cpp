#include <spriterenderer.hh>
#include <shaderprogram.hh>
#include <sprite.hh>
#include <texture.hh>
#include <glm/gtc/matrix_transform.hpp>

SpriteRenderer::SpriteRenderer()
{
    GLfloat vertices[] =
    {
        // pos....texCoord
        0.f, 0.f, 0.f, 0.f,
        1.f, 0.f, 1.f, 0.f,
        1.f, 1.f, 1.f, 1.f,
        1.f, 1.f, 1.f, 1.f,
        0.f, 1.f, 0.f, 1.f,
        0.f, 0.f, 0.f, 0.f
    };

    vbo.bind(GL_ARRAY_BUFFER);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    vao.bind();
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);
}

void SpriteRenderer::render(const ShaderProgram& shader, const Sprite& sprite) const
{
    vao.bind();
    shader.bind();
    sprite.texture->bind();

    glm::mat4 model;

    model = glm::translate(model, glm::vec3(sprite.position, 0.f));
    model = glm::rotate(model, sprite.rotation, glm::vec3(0.f, 0.f, -1.f));
    model = glm::scale(model, glm::vec3(sprite.size, 1.f));

    glUniformMatrix4fv(shader.getUniLoc("model"), 1, GL_FALSE, &model[0][0]);
    auto col = sprite.color;
    glUniform4f(shader.getUniLoc("spriteColor"), col.x/255.f, col.y/255.f, col.z/255.f, col.w);

    auto coords = sprite.texCoords;
    auto& tex = sprite.texture;
    glm::vec2 texSize(GLfloat(coords.z)/GLfloat(tex->getSize().x), GLfloat(coords.w)/GLfloat(tex->getSize().y));
    glm::vec2 texShift(GLfloat(coords.x)/GLfloat(tex->getSize().x), GLfloat(coords.y)/GLfloat(tex->getSize().y));
    glUniform2f(shader.getUniLoc("texSize"), texSize.x, texSize.y);
    glUniform2f(shader.getUniLoc("texShift"), texShift.x, texShift.y);

    glDrawArrays(GL_TRIANGLES, 0, 6);
}
