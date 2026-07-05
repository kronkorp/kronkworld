#include "../include/kronkworld/Kronkworld.hpp"
#include <cstddef>
#include <iostream>

int main(void)
{
    kw::world world;

    for (size_t i = 0; i < 100; ++i) {
        kw::Entity e = world.create();

        std::cout << "New entity : " << e << std::endl;
        if (! (i % 10)) {
            world.remove(e);
        }
    }
    return 0;
}
