#ifndef WINDOW_HH
#define WINDOW_HH

#include <glm/vec2.hpp>
#include <SFML/Window/Window.hpp>
#define GLEW_NO_GLU
#include <GL/glew.h>
#include <string>

class Window
{
public:
    Window(unsigned width, unsigned height, const std::string& title);
    Window(const Window&) = delete;
    Window& operator=(const Window&) = delete;

    void display();
    void clear() const;
    glm::uvec2 getSize() const;
    sf::Window& getWindow();

    template<typename T>
    auto pollEvent(T&& param);

private:
    sf::Window win;

    void initGlew() const;
    void printContextAttribs() const;
};

inline void Window::display()
{
    win.display();
}

inline void Window::clear() const
{
    glClear(GL_COLOR_BUFFER_BIT);
}

inline glm::uvec2 Window::getSize() const
{
    return glm::uvec2(win.getSize().x, win.getSize().y);
}

inline sf::Window& Window::getWindow()
{
    return win;
}

template<typename T>
inline auto Window::pollEvent(T&& param)
{
    return win.pollEvent(std::forward<T>(param));
}

#endif // WINDOW_HH
