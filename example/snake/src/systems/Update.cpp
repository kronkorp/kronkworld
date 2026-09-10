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

    while (win.window.pollEvent(evt)) {
        if (evt.type == sf::Event::Closed) {
            world.stop();
            return false;
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

bool MovementUpdateSystem::handle(kw::World& world)
{
    auto view = world.view<Body, Velocity>();
    auto& dt = world.getResource<Dt>();

    view.foreach([&dt](kw::Entity, Body& body, Velocity& vel){
        body.rect.move(vel.x * dt.val, vel.y * dt.val);
    });

    return true;
}
