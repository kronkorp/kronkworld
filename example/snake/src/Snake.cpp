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
#include "systems/StartupSystems.hpp"
#include "systems/UpdateSystems.hpp"
#include "systems/RenderSystems.hpp"

int main(
    [[maybe_unused]] int argc,
    [[maybe_unused]] char *const *argv
)
{
    kw::World world;

    // NOTE: The wanted way to use kronkworld is like that:
    world
        .addSystem(Stages::Startup,     std::make_unique<ResourcesStartupSystem>(), 0, 0)
        .addSystem(Stages::Startup,     std::make_unique<SnakeStartupSystem>(), 0, 0)
        .addSystem(Stages::Startup,     std::make_unique<AppleStartupSystem>(), 0, 0)
        .addSystem(Stages::ScanEvents,  std::make_unique<WindowEventSystem>())
        .addSystem(Stages::PreUpdate,   std::make_unique<PlayerInputApplySystem>())
        .addSystem(Stages::PreUpdate,   std::make_unique<TimeSystem>())
        .addSystem(Stages::Update,      std::make_unique<MovementUpdateSystem>())
        .addSystem(Stages::PostUpdate,  std::make_unique<AppleCollisionSystem>())
        .addSystem(Stages::PreRender,   std::make_unique<WindowClearSystem>())
        .addSystem(Stages::PreRender,   std::make_unique<TextsRenderSystem>())
        .addSystem(Stages::Render,      std::make_unique<WindowRenderSystem>())
        .addSystem(Stages::Render,      std::make_unique<HUDRenderSystem>())
        .addSystem(Stages::PostRender,  std::make_unique<WindowDisplaySystem>())
        .run();
    return 0;
}
