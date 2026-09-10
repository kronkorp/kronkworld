#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

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
