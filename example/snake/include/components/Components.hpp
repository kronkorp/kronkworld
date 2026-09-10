#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

#pragma once

struct Velocity
{
    float x;
    float y;
};

struct Body
{
    sf::RectangleShape rect;

    template<typename ...Args>
    explicit Body(Args&&... args) : rect(std::forward<Args>(args)...)
    {
        rect.setFillColor(sf::Color::Green);
    }
};
