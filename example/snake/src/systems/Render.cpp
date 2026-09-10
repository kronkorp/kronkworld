#include "systems/Systems.hpp"
#include "resources/Resources.hpp"
#include "components/Components.hpp"

bool WindowRenderSystem::handle(kw::World& world)
{
    auto& w = world.getResource<Window>();
    auto& score = world.getResource<Score>();

    auto q = world.view<Body>();
    // TODO: Maybe get components directly from view iterator...
    // for (auto e : q) {
    //     w.window.draw(world.get<Body>(e).rect);
    // }
    w.window.clear();
    q.foreach([&w](kw::Entity, Body& b){
        w.window.draw(b.rect);
    });
    w.window.draw(score.text);
    w.window.display();
    return true;
}
