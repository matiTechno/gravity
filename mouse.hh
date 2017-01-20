#ifndef MOUSE_HH
#define MOUSE_HH

#include <glm/glm.hpp>
#include <input.hh>
#include <viewport.hh>

template<typename T>
class WinMouse
{
public:
    const glm::vec2& getPosition() const;

    const glm::vec2& getPrevPosition() const;

    float getWheelDelta() const;

    bool wasButtonPressed(T button) const;

    bool wasButtonReleased(T button) const;

    bool isButtonPressed(T button) const;

    bool isButtonPressed_in_viewport(T button, const glm::vec4& viewport);

    // call before gathering input
    void update();

    void setPosition(const glm::vec2& position);

    void setWheelDelta(float delta);

    void buttonUpEvent(T button);

    void buttonDownEvent(T button);

    glm::vec2 getMove() const;

private:
    glm::vec2 position;
    glm::vec2 prevPosition;
    float wheelDelta;
    Input<T, std::hash<std::underlying_type_t<T>>> buttons;
};

template<typename T>
inline const glm::vec2& WinMouse<T>::getPosition() const
{
    return position;
}

template<typename T>
inline const glm::vec2& WinMouse<T>::getPrevPosition() const
{
    return prevPosition;
}

template<typename T>
inline float WinMouse<T>::getWheelDelta() const
{
    return wheelDelta;
}

template<typename T>
inline bool WinMouse<T>::wasButtonPressed(T button) const
{
    return buttons.wasKeyPressed(std::forward<T>(button));
}

template<typename T>
inline bool WinMouse<T>::wasButtonReleased(T button) const
{
    return buttons.wasKeyReleased(std::forward<T>(button));
}

template<typename T>
inline bool WinMouse<T>::isButtonPressed(T button) const
{
    return buttons.isKeyPressed(std::forward<T>(button));
}

template<typename T>
inline bool WinMouse<T>::isButtonPressed_in_viewport(T button, const glm::vec4& viewport)
{
    if(isButtonPressed(button) && is_in_viewport(position, viewport))
        return true;
    return false;
}

// call before gathering input
template<typename T>
inline void WinMouse<T>::update()
{
    buttons.update();
    wheelDelta = 0.f;
    prevPosition = position;
//    entered = false;
//    left = false;
}

template<typename T>
inline void WinMouse<T>::setPosition(const glm::vec2& position)
{
    this->position = position;
}

template<typename T>
inline void WinMouse<T>::setWheelDelta(float delta)
{
    wheelDelta = delta;
}

template<typename T>
inline void WinMouse<T>::buttonUpEvent(T button)
{
    buttons.keyUpEvent(std::forward<T>(button));
}

template<typename T>
inline void WinMouse<T>::buttonDownEvent(T button)
{
    buttons.keyDownEvent(std::forward<T>(button));
}

template<typename T>
inline glm::vec2 WinMouse<T>::getMove() const
{
    return position - prevPosition;
}

#endif // MOUSE_HH
