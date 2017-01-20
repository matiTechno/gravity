#include <shaderprogram.hh>
#include <vao.hh>
#include <window.hh>
#include <SFML/Window.hpp>
#include <chrono>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <iostream>
#include <sstream>
#include <future>
#include <orthocamera.hh>
#include <mouse.hh>
#include <spriterenderer.hh>
#include <sprite.hh>
#include <texture.hh>

struct Particle
{
    glm::vec2 position;
    glm::vec2 acceleration;
    glm::vec2 velocity;

    void update(float dt)
    {
        float lenVel = glm::length(velocity);
        if(lenVel > 0.f)
        {
            acceleration -= glm::normalize(velocity) * drag * glm::pow(lenVel, 2.f);
        }
        if(glm::length(acceleration) > maxAcc)
            acceleration = glm::normalize(acceleration) * maxAcc;

        position += velocity * dt + 0.5f * acceleration * glm::pow(dt, 2.f);
        velocity += acceleration * dt;
    }

    constexpr static float drag = 0.01f;
    constexpr static float maxAcc = 10000.f;
};

struct VboData
{
    glm::vec2 position;
    glm::vec4 color;
};

void set_mandelbrot(const glm::vec2& pos, const glm::vec2& size, std::vector<Particle>& particles)
{
    float aspect = size.x / size.y;
    std::size_t in_row = static_cast<std::size_t>(glm::sqrt(aspect * particles.size()));
    std::size_t num_rows = particles.size() / in_row;

    for(std::size_t i = 0; i < particles.size(); ++i)
    {
        particles[i].position = pos + glm::vec2((i % in_row) / float(in_row - 1) * size.x, float(i / in_row) / num_rows * size.y);

        glm::vec2 pos_map(-2.5f + 3.5f / size.x * (particles[i].position.x - pos.x),
                          -1.f + 2.f / size.y * (particles[i].position.y - pos.y));

        std::size_t iterations = 100;
        std::size_t iteration = 0;
        glm::vec2 z(0.f, 0.f);

        while(glm::length(z) < 4.f && iteration <= iterations)
        {
            float xtemp = glm::pow(z.x, 2.f) - glm::pow(z.y, 2.f) + pos_map.x;
            z.y = 2.f * z.x * z.y + pos_map.y;
            z.x = xtemp;
            ++iteration;
        }

        if(iteration < iterations)
            particles[i].position.x += size.x;
    }
}

bool isUnsigned(const char* ptr)
{
    while(*ptr)
    {
        if(*ptr < '0' || *ptr > '9')
            return false;
        ++ptr;
    }
    return true;
}

void update(std::vector<Particle>& particles, VboData* vboData,
            std::size_t beg, std::size_t end,
            const glm::vec2& pos, bool isActive,
            float frameTime, float gravity)
{
    std::size_t len = end - beg;
    if(len > 100000)
    {
        std::size_t mid = beg + len / 2;

        auto fut = std::async(std::launch::async, update, std::ref(particles), vboData, mid, end,
                              std::ref(pos), isActive, frameTime, gravity);

        update(particles, vboData, beg, mid, pos, isActive, frameTime, gravity);
        fut.get();
        return;
    }

    for(std::size_t i = beg; i < end; ++i)
    {
        if(isActive)
        {
            glm::vec2 distVec = pos - particles[i].position;
            float distLen = glm::length(distVec);
            glm::vec2 distDir = glm::normalize(distVec);
            particles[i].acceleration = distDir * gravity / glm::pow(distLen, 2.f);
        }
        else
            particles[i].acceleration = glm::vec2(0.f, 0.f);

        particles[i].update(frameTime);

        vboData[i].position = particles[i].position;
        vboData[i].color = glm::vec4(1.f, 0.3f, length(particles[i].acceleration) / Particle::maxAcc, 0.2f);
    }
}

struct SpriteMouse
{
    std::pair<Texture*, glm::uvec4> m_default, m_grab, m_point, m_active;
    float scale;
    glm::vec2 position;
    bool isActive = false;
    bool lock1 = false;
    bool lock2 = false;
    Sprite getSprite(float cameraZoom) const
    {
        Sprite sprite;
        sprite.texCoords = m_active.second;
        sprite.size = glm::vec2(m_active.second.z, m_active.second.w) * scale / cameraZoom;
        sprite.position = position;
        sprite.texture = m_active.first;
        return sprite;
    }
    void reset()
    {
        m_active = m_default;
    }
};

int main(int argv, char* argc[])
{
    std::size_t numParticles;
    if(argv == 1)
        numParticles = 200000;
    else if(argv == 2)
    {
        if(!isUnsigned(argc[1]))
        {
            std::cout << "specify num particles or invoke program without argument" << std::endl;
            return 0;
        }
        std::stringstream stream(argc[1]);
        stream >> numParticles;
    }
    else
    {
        std::cout << "specify num particles or invoke program without argument" << std::endl;
        return 0;
    }

    constexpr float aspect = 800.f / 600.f;
    Window window(800, 600, "gravity");
    window.getWindow().setMouseCursorVisible(false);
    glm::vec4 viewport = getViewport(aspect, window.getWindow());
    OrthoCamera camera(0.f, 0.f, 800.f, 600.f, viewport);
    assert(camera.getAspect() == aspect);
    WinMouse<sf::Mouse::Button> winMouse;
    TexMan textures("res");

    SpriteMouse spriteMouse;
    spriteMouse.scale = 0.6f;
    spriteMouse.m_default = {&textures.get("pointer.png"), glm::uvec4(15, 3, 71, 72)};
    spriteMouse.m_grab = {&textures.get("pointer.png"), glm::uvec4(119, 3, 61, 68)};
    spriteMouse.m_point = {&textures.get("pointer.png"), glm::uvec4(213, 3, 67, 72)};

    ShaderProgram sprite_shader("res/default_shader");
    ShaderProgram shader("res/gravity_shader");

    SpriteRenderer renderer;

    glClearColor(20.f / 255.f, 3.f / 255.f, 20.f / 255.f, 1.f);
    glEnable(GL_BLEND);

    constexpr float gravity = 3000000.f;

    BO vbo;
    vbo.bind(GL_ARRAY_BUFFER);
    glBufferData(GL_ARRAY_BUFFER, GLsizeiptr(numParticles * sizeof(VboData)), nullptr, GL_DYNAMIC_DRAW);

    VAO vao;
    vao.bind();
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(VboData), 0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(VboData), reinterpret_cast<const void*>(sizeof(glm::vec2)));
    glEnableVertexAttribArray(1);

    std::vector<Particle> particles;
    particles.reserve(numParticles);

    for(std::size_t i = 0; i < numParticles; ++i)
    {
        Particle particle;
        particle.velocity = glm::vec2(0.f, 0.f);
        particle.acceleration = glm::vec2(0.f, 0.f);
        particles.push_back(std::move(particle));
    }

    glm::vec2 size(350.f, 200.f);
    set_mandelbrot(camera.getPosition() + camera.getRange() / 2.f - size / 2.f, size, particles);

    constexpr float maxDt = 0.016f;
    auto currentTime = std::chrono::high_resolution_clock::now();
    bool isRunning = true;

    while(isRunning)
    {
        spriteMouse.reset();
        winMouse.update();
        sf::Event event;
        while(window.pollEvent(event))
        {
            if(event.type == sf::Event::Closed)
                isRunning = false;
            else if(event.type == sf::Event::Resized)
            {
                glm:: vec4 temp_viewport = getViewport(aspect, window.getWindow());
                float scale = temp_viewport.z / viewport.z;
                spriteMouse.scale *= 1.f / scale;
                viewport = temp_viewport;
            }
            else if(event.type == sf::Event::KeyPressed)
            {
                if(event.key.code == sf::Keyboard::Escape)
                    isRunning = false;
            }
            else if(event.type == sf::Event::MouseButtonPressed && window.getWindow().hasFocus())
            {
                glm::vec2 pos(event.mouseButton.x, event.mouseButton.y);
                if(is_in_viewport(pos, viewport))
                    winMouse.buttonDownEvent(event.mouseButton.button);
            }
            else if(event.type == sf::Event::MouseWheelScrolled && window.getWindow().hasFocus())
            {
                glm::vec2 pos(event.mouseWheelScroll.x, event.mouseWheelScroll.y);
                if(is_in_viewport(pos, viewport))
                    winMouse.setWheelDelta(event.mouseWheelScroll.delta);
            }
            else if(event.type == sf::Event::MouseButtonReleased)
            {
                winMouse.buttonUpEvent(event.mouseButton.button);
            }
            else if(event.type == sf::Event::MouseMoved && window.getWindow().hasFocus())
            {
                winMouse.setPosition(glm::vec2(event.mouseMove.x, event.mouseMove.y));
            }
        }

        if(winMouse.isButtonPressed_in_viewport(sf::Mouse::Left, viewport) && !spriteMouse.lock2)
        {
            spriteMouse.m_active = spriteMouse.m_point;
            spriteMouse.isActive = true;
            spriteMouse.lock1 = true;
        }
        else
            spriteMouse.isActive = false;
        if(winMouse.isButtonPressed(sf::Mouse::Right) && !spriteMouse.lock1)
        {
            camera.movePosition(camera.get_mouse_world_position(winMouse.getPrevPosition()) -
                                camera.get_mouse_world_position(winMouse.getPosition()));
            spriteMouse.m_active = spriteMouse.m_grab;
            spriteMouse.lock2 = true;
        }
        float delta = winMouse.getWheelDelta();
        if(glm::abs(delta) > 0.f)
        {
            if(winMouse.isButtonPressed(sf::Mouse::Right) || winMouse.isButtonPressed(sf::Mouse::Left))
            {
                if(delta > 0.f)
                    camera.zoom_to_mouse(glm::pow(1.2f, delta), winMouse.getPosition());
                else
                    camera.zoom_to_mouse(glm::pow((1.f / 1.2f), -delta), winMouse.getPosition());
            }
            else
            {
                if(delta > 0.f)
                    camera.zoom_to_center(glm::pow(1.2f, delta));
                else
                    camera.zoom_to_center(glm::pow(1.f / 1.2f, -delta));
            }

        }

        if(winMouse.wasButtonReleased(sf::Mouse::Left))
            spriteMouse.lock1 = false;
        if(winMouse.wasButtonReleased(sf::Mouse::Right))
            spriteMouse.lock2 = false;
        if(is_in_viewport(winMouse.getPosition(), viewport))
        {
            window.getWindow().setMouseCursorVisible(false);
            spriteMouse.position = camera.get_mouse_world_position(winMouse.getPosition());
        }
        else
        {
            window.getWindow().setMouseCursorVisible(true);
        }

        auto newTime = std::chrono::high_resolution_clock::now();
        float frameTime = std::chrono::duration<float>(newTime - currentTime).count();
        currentTime = newTime;

        if(frameTime > maxDt)
            frameTime = maxDt;

        vbo.bind(GL_ARRAY_BUFFER);
        VboData* vboData = reinterpret_cast<VboData*>(glMapBufferRange(GL_ARRAY_BUFFER, 0, GLsizei(numParticles * sizeof(VboData)), GL_MAP_WRITE_BIT
                                                                       | GL_MAP_INVALIDATE_BUFFER_BIT | GL_MAP_UNSYNCHRONIZED_BIT));

        update(particles, vboData, 0, particles.size(), camera.get_mouse_world_position(winMouse.getPosition()),
               spriteMouse.isActive, frameTime, gravity);

        glUnmapBuffer(GL_ARRAY_BUFFER);

        auto projection = camera.getProjetion();

        setViewport(window.getWindow());
        glClear(GL_COLOR_BUFFER_BIT);
        setViewport(viewport);

        glDisable(GL_BLEND);

        sprite_shader.bind();
        glUniformMatrix4fv(sprite_shader.getUniLoc("projection"), 1, GL_FALSE, &projection[0][0]);

        Sprite sp1;
        sp1.size = camera.getRange();
        sp1.position = camera.getPosition();
        sp1.texture = &textures.get("black.png");
        sp1.texCoords = glm::vec4(0, 0, sp1.texture->getSize().x, sp1.texture->getSize().y);

        renderer.render(sprite_shader, sp1);

        glEnable(GL_BLEND);

        glBlendFunc(GL_SRC_ALPHA, GL_ONE);

        shader.bind();
        glUniformMatrix4fv(shader.getUniLoc("projection"), 1, GL_FALSE, &projection[0][0]);
        vao.bind();
        glDrawArrays(GL_POINTS, 0, GLsizei(numParticles));

        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        sprite_shader.bind();
        renderer.render(sprite_shader, spriteMouse.getSprite(camera.getZoom()));

        window.display();
    }
    return 0;
}
