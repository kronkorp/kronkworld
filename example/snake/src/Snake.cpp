#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <chrono>
#include <iostream>
#include <memory>
#include "../../../include/kronkworld/Kronkworld.hpp"
#include <SFML/Graphics/RenderWindow.hpp>
#include "SFML/Graphics.hpp"
#include <utility>

struct Dt
{
    float val;
};

struct Window
{
    sf::RenderWindow  window;

    template<typename ...Args>
    explicit Window(Args&&... args) : window(std::forward<Args>(args)...) {}
};

struct Body
{
    sf::RectangleShape rect;

    template<typename ...Args>
    explicit Body(Args&&... args) : rect(std::forward<Args>(args)...)
    {
        rect.setFillColor(sf::Color::Green);
    }
};

class TimeSystem : public kw::ISystem
{
public:
    void handle(kw::World& world) override
    {
        auto ctime = std::chrono::high_resolution_clock::now();

        std::chrono::duration<float> frameTime = ctime - m_lastTime;
        m_lastTime = ctime;

        auto& dt = world.getResource<Dt>();
        dt.val = frameTime.count();
        std::cout << "dt is : " << dt.val << " s" << std::endl;
    }

    private:
        std::chrono::high_resolution_clock::time_point m_lastTime = std::chrono::high_resolution_clock::now();
};

class WindowSystem : public kw::ISystem
{
    public:
        void handle(kw::World& world) override
        {
            auto& w = world.getResource<Window>();

            auto q = world.view<Body>();
            // TODO: Maybe get components directly from view iterator...
            // for (auto e : q) {
            //     w.window.draw(world.get<Body>(e).rect);
            // }
            q.foreach([&w](kw::Entity, Body& b){
                w.window.draw(b.rect);
            });
            w.window.display();
        }
};

int main(
    [[maybe_unused]] int argc,
    [[maybe_unused]] char *const *argv
)
{
    kw::World world;

    world.addResource<Dt>();
    world.addResource<Window>(sf::VideoMode(800, 600), "feur");
    auto head = world.create();
    world.add<Body>(head, sf::Vector2f{20, 20});
    world.addUpdate(std::make_unique<TimeSystem>());
    world.addRender(std::make_unique<WindowSystem>());
    while (1) {
        world.runOnce();
    }
    return 0;
}
