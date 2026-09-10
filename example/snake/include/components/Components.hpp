#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <utility>

#pragma once

struct SnakeHead {};
struct SnakeBody {};
struct Apple {};

struct Speed
{
    float speed;
};

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

enum class Directions
{
    Up,
    Right,
    Down,
    Left
};

struct PlayerInput
{
    Directions direction;
};

struct FpsText {};
struct ScoreText {};

struct Text
{
    sf::Text text;

    template<typename ...Args>
    explicit Text(Args&&... args) : text(std::forward<Args>(args)...) {}
};
