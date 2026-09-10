#include "systems/Systems.hpp"
#include "resources/Resources.hpp"
#include "components/Components.hpp"

bool StartupSystem::handle(kw::World& world)
{
    world.addResource<Dt>();
    world.addResource<Window>(sf::VideoMode(800, 600), "Snake - Demo");

    auto head = world.create();
    world.add<Body>(head, sf::Vector2f{20, 20});
    world.add<Velocity>(head, 100.f, 0.f);

    return false;
}
