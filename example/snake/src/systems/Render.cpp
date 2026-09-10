#include "kronkworld/entity/Entity.hpp"
#include "kronkworld/world/World.hpp"
#include "systems/RenderSystems.hpp"
#include "resources/Resources.hpp"
#include "components/Components.hpp"
#include <SFML/Graphics/Color.hpp>
#include <format>

bool WindowRenderSystem::handle(kw::World& world)
{
    auto& w = world.getResource<Window>();

    auto q = world.view<Body>();
    q.foreach([&w](kw::Entity, Body& b){
        w.window.draw(b.rect);
    });
    return true;
}

bool HUDRenderSystem::handle(kw::World& world)
{
    auto view = world.view<Text>();
    auto& w = world.getResource<Window>();

    view.foreach([&w](kw::Entity, Text& text) {
        w.window.draw(text.text);
    });
    return true;
}

bool WindowDisplaySystem::handle(kw::World& world)
{
    auto& w = world.getResource<Window>();

    w.window.display();
    return true;
}

bool WindowClearSystem::handle(kw::World& world)
{
    auto& w = world.getResource<Window>();
    w.window.clear(sf::Color::Black);
    return true;
}

bool TextsRenderSystem::handle(kw::World& world)
{
    auto score = world.view<ScoreText>().first();
    world.get<Text>(score).text.setString(std::format("Score: {}", world.getResource<Score>().score));

    auto fps = world.view<FpsText>().first();
    world.get<Text>(fps).text.setString(std::format("FPS: {:.1f}", world.getResource<FPS>().fps));
    return true;
}
