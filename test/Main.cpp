#include "../include/kronkworld/Kronkworld.hpp"
#include <cstddef>
#include <iostream>

typedef struct {

    float x;
    float y;

} Position;

typedef struct {

    float v;

} Velocity;

int main(void)
{
    kw::world world;

    try {
        for (size_t i = 0; i < 50; ++i) {
            kw::Entity e = world.create();

            std::cout << "New entity : " << e << std::endl;
            if (! (i % 10)) {
                world.remove(e);
            }
        }
    } catch (const kw::EntityError& e) {
        std::cout << "too many entities" << std::endl;
    }
    auto i1 = world.create();
    auto i2 = world.create();
    auto c1 = world.add<Position>(i1);
    auto c3 = world.add<Velocity>(i1);
    auto c2 = world.add<Position>(i2);
    std::cout << "c1 : " << c1 << ", c2: " << c2 << std::endl;
    std::cout << "c3 : " << c3 << std::endl;
    return 0;
}
