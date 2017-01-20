#include <window.hh>
#include <iostream>
#include <SFML/Window/VideoMode.hpp>

Window::Window(unsigned width, unsigned height, const std::string& title):
    win(sf::VideoMode(width, height), title, sf::Style::Default,
        sf::ContextSettings(0, 0, 4, 3, 3, sf::ContextSettings::Core))
{
    auto vMode = sf::VideoMode::getDesktopMode();
    win.setPosition(sf::Vector2i(vMode.width/2 - width/2, vMode.height/2 - height/2));
    //win.setVerticalSyncEnabled(true);
    win.setKeyRepeatEnabled(false);
    printContextAttribs();
    initGlew();
}

void Window::initGlew() const
{
    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if (GLEW_OK != err)
        throw std::runtime_error("glewInit failed");

    std::cout << "using GLEW: " << glewGetString(GLEW_VERSION) << std::endl;
}

void Window::printContextAttribs() const
{
    auto attribs = win.getSettings();

    std::cout << "DEPTH BITS: " << attribs.depthBits << std::endl;
    std::cout << "STENCIL BITS: " << attribs.stencilBits << std::endl;
    std::cout << "SAMPLES: " << attribs.antialiasingLevel << std::endl;
    std::cout << "MAJOR VERSION: " << attribs.majorVersion << std::endl;
    std::cout << "MINOR VERSION: " << attribs.minorVersion << std::endl;
}
