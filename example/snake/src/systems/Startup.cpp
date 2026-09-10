#include "systems/Systems.hpp"
#include "resources/Resources.hpp"
#include "components/Components.hpp"
#include <SFML/System/Vector2.hpp>

bool StartupSystem::handle(kw::World& world)
{
    world.addResource<Dt>();
    auto& win = world.addResource<Window>(sf::VideoMode(800, 600), "Snake - Demo");

    auto head = world.create();
    world.add<Body>(head, sf::Vector2f{20, 20}).rect.setPosition(win.window.getSize().x / 2 - 10, win.window.getSize().y / 2 - 10);
    world.add<Velocity>(head, 1.f, 0.f);
    world.add<PlayerInput>(head, Directions::Up);
    world.add<Speed>(head, 100.f);

    auto apple = world.create();

    return false;
}
