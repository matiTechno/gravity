#ifndef INPUT_HH
#define INPUT_HH

#include <unordered_map>

template<typename T, typename Hash>
class Input
{
public:
    void update(); // call before gathering input
    void keyDownEvent(T code);
    void keyUpEvent(T code);

    bool wasKeyPressed(T code) const;
    bool wasKeyReleased(T code) const;
    bool isKeyPressed(T code) const;

private:
    std::unordered_map<T, bool, Hash> keys1;
    std::unordered_map<T, bool, Hash> keys2;
    std::unordered_map<T, bool, Hash> keys3;
};

template<typename T, typename Hash>
void Input<T, Hash>::update()
{
    keys1.clear();
    keys2.clear();
}

template<typename T, typename Hash>
void Input<T, Hash>::keyDownEvent(T code)
{
    keys1[code] = true;
    keys3[code] = true;
}

template<typename T, typename Hash>
void Input<T, Hash>::keyUpEvent(T code)
{
    keys2[code] = true;
    keys3.erase(code);
}

template<typename T, typename Hash>
bool Input<T, Hash>::wasKeyPressed(T code) const
{
    auto i = keys1.find(code);

    if(i == keys1.end())
        return false;
    else
        return true;
}

template<typename T, typename Hash>
bool Input<T, Hash>::wasKeyReleased(T code) const
{
    auto i = keys2.find(code);

    if(i == keys2.end())
        return false;
    else
        return true;
}

template<typename T, typename Hash>
bool Input<T, Hash>::isKeyPressed(T code) const
{
    auto i = keys3.find(code);

    if(i == keys3.end())
        return false;
    else
        return true;
}

#endif // INPUT_HH
