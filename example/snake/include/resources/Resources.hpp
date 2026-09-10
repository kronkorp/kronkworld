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
#include <cstddef>
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

    size_t score = 0;
    sf::Text text;

    template<typename ...Args>
    explicit Score(Args&&... args) : text(std::forward<Args>(args)...) {}

};

struct Font
{
    sf::Font font;

    template<typename ...Args>
    explicit Font(Args&&... args) : font(std::forward<Args>(args)...) {}
};
