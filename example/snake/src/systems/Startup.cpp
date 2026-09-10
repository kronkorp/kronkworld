#include "systems/Systems.hpp"
#include "resources/Resources.hpp"
#include "components/Components.hpp"
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/System/Vector2.hpp>
#include <cstdlib>
#include <format>

bool StartupSystem::handle(kw::World& world)
{
    world.addResource<Dt>();

    auto& win = world.addResource<Window>(sf::VideoMode(800, 600), "Snake - Demo");

    auto& font = world.addResource<Font>();
    font.font.loadFromFile("res/fonts/OpenSans-Bold.ttf");

    auto& score = world.addResource<Score>();
    score.text.setPosition(sf::Vector2f{0, 0});
    score.text.setFont(font.font);
    score.text.setFillColor(sf::Color::White);
    score.text.setString(std::format("Score: {}", score.score));

    auto head = world.create();
    world.add<Body>(head, sf::Vector2f{20, 20}).rect.setPosition(win.window.getSize().x / 2 - 10, win.window.getSize().y / 2 - 10);
    world.add<Velocity>(head, 1.f, 0.f);
    world.add<PlayerInput>(head, Directions::Up);
    world.add<Speed>(head, 100.f);
    world.add<SnakeHead>(head);

    auto apple = world.create();
    world.add<Body>(apple, sf::Vector2f{20, 20}).rect.setPosition((rand() % (win.window.getSize().x / 20)) * 20, (rand() % (win.window.getSize().y / 20)) * 20);
    world.get<Body>(apple).rect.setFillColor(sf::Color::Red);
    world.add<Apple>(apple);

    return false;
}
