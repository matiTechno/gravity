#ifndef VIEWPORT_HH
#define VIEWPORT_HH

#include <glm/vec4.hpp>
#include <glm/vec2.hpp>
#include <SFML/Window/Window.hpp>

glm::vec4 getViewport(float aspect, const sf::Window& window);

void setViewport(const sf::Window& window);

void setViewport(const glm::vec4& viewport);

bool is_in_viewport(const glm::vec2& pos, const glm::vec4& viewport);

#endif // VIEWPORT_HH
