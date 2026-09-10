#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <cmath>
#include <cstddef>
#include <memory>
#include <utility>

#pragma once

struct Dt
{
    float val;
};

struct Window
{
    sf::RenderWindow  window;

    template<typename ...Args>
    explicit Window(Args&&... args) : window(std::forward<Args>(args)...) {}
};

struct Score
{

    size_t score;

};

struct Font
{
    std::unique_ptr<sf::Font> font = std::make_unique<sf::Font>();;

};

struct FPS
{
    long double fps;
};
