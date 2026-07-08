#include "../include/kronkworld/Kronkworld.hpp"
#include <cstddef>
#include <iostream>
#include <memory>

typedef struct {

    float x;
    float y;

} Position;

typedef struct {

    float v;

} Velocity;

class GreetSystem : public kw::ISystem
{
    public:
        void handle(kw::World& world) override
        {
            auto view = world.view<Position, Velocity>();

            for (auto e : view) {
                auto& pos = view.get<Position>(e);
                std::cout << "Hello entt with velocity : " << e << "v = " << pos.x << ", " << pos.y << std::endl;
            }
        }
};

int main(void)
{
    auto world = std::make_unique<kw::World>();

    try {
        for (size_t i = 0; i < 50; ++i) {
            kw::Entity e = world->create();

            std::cout << "New entity : " << e << std::endl;
            if (! (i % 10)) {
                world->remove(e);
            }
        }
    } catch (const kw::EntityError& e) {
        std::cout << "too many entities" << std::endl;
    }
    auto i1 = world->create();
    auto i2 = world->create();
    auto c1 = world->add<Position>(i1, 100.f, 100.f);
    auto c3 = world->add<Velocity>(i1);
    auto c2 = world->add<Position>(i2, 1.f, -82.f);
    std::cout << "c1 : " << c1.x << ", c2: " << c2.x << std::endl;
    std::cout << "c3 : " << c3.v << std::endl;

    world->get<Position>(i1).x = 100;

    world->show(i1);
    world->show(i2);

    std::cout << "i1 pos x : " << world->get<Position>(i1).x << std::endl;
    std::cout << "i2 pos x : " << world->get<Position>(i2).x << std::endl;

    std::cout << "i1 has Velocity : " << world->has<Velocity>(i1) << std::endl;
    std::cout << "i2 has Velocity : " << world->has<Velocity>(i2) << std::endl;

    world->remove<Velocity>(i1);

    std::cout << "i1 has Velocity : " << world->has<Velocity>(i1) << std::endl;
    std::cout << "i2 has Velocity : " << world->has<Velocity>(i2) << std::endl;

    world->addUpdate(std::make_unique<GreetSystem>());

    world->add<Velocity>(i1, 100.f);

    for (size_t i = 0; i < 100; ++i) {
        world->runOnce();
    }

    return 0;
}
