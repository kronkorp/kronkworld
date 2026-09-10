#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <iostream>
#include "components/Components.hpp"
#include "kronkworld/entity/Entity.hpp"
#include "kronkworld/world/World.hpp"
#include "systems/Systems.hpp"
#include "resources/Resources.hpp"

bool WindowEventSystem::handle(kw::World& world)
{
    auto& win = world.getResource<Window>();
    sf::Event evt;
    auto e = world.view<PlayerInput>().first();

    while (win.window.pollEvent(evt)) {
        if (evt.type == sf::Event::Closed) {
            world.stop();
            return false;
        }
        if (evt.type == sf::Event::KeyPressed) {
            switch (evt.key.code) {
                case sf::Keyboard::Key::Up:
                    world.get<PlayerInput>(e).direction = Directions::Up; break;
                case sf::Keyboard::Key::Down:
                    world.get<PlayerInput>(e).direction = Directions::Down; break;
                case sf::Keyboard::Key::Right:
                    world.get<PlayerInput>(e).direction = Directions::Right; break;
                case sf::Keyboard::Key::Left:
                    world.get<PlayerInput>(e).direction = Directions::Left; break;
                default:
                    continue;
            }
        }
    }
    return true;
}

bool TimeSystem::handle(kw::World& world)
{
    auto ctime = std::chrono::high_resolution_clock::now();

    std::chrono::duration<float> frameTime = ctime - m_lastTime;
    m_lastTime = ctime;

    auto& dt = world.getResource<Dt>();
    dt.val = frameTime.count();
    std::cout << "dt is : " << dt.val << " s" << std::endl;
    return true;
}

bool PlayerInputApplySystem::handle(kw::World& world)
{
    auto view = world.view<PlayerInput, Velocity>();

    view.foreach([](kw::Entity, PlayerInput& input, Velocity& vel){
        switch (input.direction) {
            case Directions::Up:
                vel.x = 0;
                vel.y = -1;
                break;
            case Directions::Down:
                vel.x = 0;
                vel.y = 1;
                break;
            case Directions::Left:
                vel.x = -1;
                vel.y = 0;
                break;
            case Directions::Right:
                vel.x = 1;
                vel.y = 0;
                break;
        }
    });
    return true;
}

bool MovementUpdateSystem::handle(kw::World& world)
{
    auto view = world.view<Body, Velocity, Speed>();
    auto& dt = world.getResource<Dt>();

    view.foreach([&dt](kw::Entity, Body& body, Velocity& vel, Speed& speed){
        body.rect.move(vel.x * dt.val * speed.speed, vel.y * dt.val * speed.speed);
    });

    return true;
}
