#include "systems/StartupSystems.hpp"
#include "resources/Resources.hpp"
#include "components/Components.hpp"
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/System/Vector2.hpp>
#include <cstddef>
#include <cstdlib>
#include <format>

bool ResourcesStartupSystem::handle(kw::World& world)
{
    world.addResource<Dt>();
    world.addResource<FPS>(0);
    world.addResource<Score>(0);

    world.addResource<Window>(sf::VideoMode(800, 600), "Snake - Demo");

    auto& font = world.addResource<Font>();
    font.font->loadFromFile("res/fonts/OpenSans-Bold.ttf");

    // Score Ctr
    auto scoreCtr = world.create();
    world.add<ScoreText>(scoreCtr);
    auto& txt2 = world.add<Text>(scoreCtr, std::format("Score: {}", 0), *font.font);
    txt2.text.setPosition(0, 0);
    txt2.text.setFillColor(sf::Color::White);

    // FPS Ctr
    auto fpsCtr = world.create();
    world.add<FpsText>(fpsCtr);
    auto& txt = world.add<Text>(fpsCtr, std::format("FPS: {:.1f}", 0.f), *font.font);
    txt.text.setPosition(0, 40);
    txt.text.setFillColor(sf::Color::White);

    return false;
}

bool SnakeStartupSystem::handle(kw::World& world)
{
    auto& win = world.getResource<Window>();

    // Head
    auto head = world.create();
    world.add<Body>(head, sf::Vector2f{20, 20}).rect.setPosition(win.window.getSize().x / 2, win.window.getSize().y / 2);
    world.add<Velocity>(head, 1.f, 0.f);
    world.add<PlayerInput>(head, Directions::Up);
    auto& history = world.add<SnakeHead>(head);

    for (size_t i = 0; i < 3; ++i) {
        auto body = world.create();
        world.add<Body>(body, sf::Vector2f{20, 20}).rect.setPosition(world.get<Body>(head).rect.getPosition() + sf::Vector2f(0, (i + 1) * 20));
        world.add<SnakeBody>(body, static_cast<std::size_t>(i + 1));
        history.history.push_back(world.get<Body>(body).rect.getPosition());
    }

    return false;
}

bool AppleStartupSystem::handle(kw::World& world)
{
    auto& win = world.getResource<Window>();

    // Apple
    auto apple = world.create();
    world.add<Body>(apple, sf::Vector2f{20, 20}).rect.setPosition((rand() % (win.window.getSize().x / 20)) * 20, (rand() % (win.window.getSize().y / 20)) * 20);
    world.get<Body>(apple).rect.setFillColor(sf::Color::Red);
    world.add<Apple>(apple);

    return false;
}
