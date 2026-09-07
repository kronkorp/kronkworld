#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
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

struct PlayerInputs
{
    
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

class WindowRenderSystem : public kw::ISystem
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
            w.window.clear();
            q.foreach([&w](kw::Entity, Body& b){
                w.window.draw(b.rect);
            });
            w.window.display();
        }
};

class WindowEventSystem : public kw::ISystem
{
    public:
        void handle(kw::World& world)
        {
            auto& win = world.getResource<Window>();
            sf::Event evt;

            while (win.window.pollEvent(evt)) {
                if (evt.type == sf::Event::Closed) {
                    world.stop();
                    return;
                }
            }
        }
};

class StartupSystem : public kw::ISystem
{
    public:
        void handle(kw::World& world) override
        {
            static int isPassed = 0;

            if (isPassed > 0) {
                return;
            }
            ++isPassed;
            world.addResource<Dt>();
            world.addResource<Window>(sf::VideoMode(800, 600), "feur");
            // NOTE: -> world.spawn(Body{sf::Vector2f{20, 20}}, ...)
            auto head = world.create();
            world.add<Body>(head, sf::Vector2f{20, 20});
        }
};

int main(
    [[maybe_unused]] int argc,
    [[maybe_unused]] char *const *argv
)
{
    kw::World world;

    world.addUpdate(std::make_unique<StartupSystem>())
        .addUpdate(std::make_unique<WindowEventSystem>())
        .addUpdate(std::make_unique<TimeSystem>())
        .addRender(std::make_unique<WindowRenderSystem>())
        .run();
    // NOTE: The wanted way to use kronkworld is like that:
    /*
     *  world.addSystem(kw::Startup, std::make_unique<StartupSystem>())
     *      .addSystem(kw::PreUpdate, std::make_unique<WindowEventSystem>())
     *      .addSystem(kw::Update, std::make_unique<TimeSystem>())
     *      .addSystem(kw::Render, std::make_unique<WindowRenderSystem>());
    */
    // NOTE: Ressources and entities (&& coomponents) will be done into startup systems
    // NOTE: Scheduler with multithreading and stage (group / priority) is needed (-> see https://github.com/kronkorp/kronkflow cuz this is what we're gonna use)
    return 0;
}
