#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <chrono>
#include <cstddef>
#include <iostream>
#include <memory>
#include "../../../include/kronkworld/Kronkworld.hpp"
#include <SFML/Graphics/RenderWindow.hpp>
#include "SFML/Graphics.hpp"
#include <utility>
#include "systems/Systems.hpp"

int main(
    [[maybe_unused]] int argc,
    [[maybe_unused]] char *const *argv
)
{
    kw::World world;

    // NOTE: The wanted way to use kronkworld is like that:
    world.addSystem(Stages::Startup, std::make_unique<StartupSystem>(), 0, 0)
        .addSystem(Stages::Update, std::make_unique<WindowEventSystem>(), 0, 1)
        .addSystem(Stages::Update, std::make_unique<TimeSystem>(), 0, 1)
        .addSystem(Stages::Update, std::make_unique<MovementUpdateSystem>(), 0, 1)
        .addSystem(Stages::Render, std::make_unique<WindowRenderSystem>(), 0, 1)
        .run();
    return 0;
}
