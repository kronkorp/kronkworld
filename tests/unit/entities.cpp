extern "C" {
    #include "kronklab/kronklab.h"
}
#include "kronkworld/Kronkworld.hpp"
#include <cstddef>

Test(entities, creation_good)
{
    kw::World world;

    for (size_t i = 0; i < 67; ++i) {
        world.create();
    }
    AssertEq(world.create(), 67, "Should incremente entities");
}

Test(entities, creation_bad)
{
    kw::World world;
    bool dummybool = false;

    try {
        for (size_t i = 0; i < 10000000; ++i) {
            world.create();
        }
    } catch (const kw::EntityError& e) {
        dummybool = true;
    }
    AssertEq(dummybool, true, "Should throw on error");
}

Test(entities, reusing)
{
    kw::World world;

    for (size_t i = 0; i < 80; ++i) {
        world.create();
    }
    world.remove(6);
    AssertEq(world.create(), 6, "Should incremente entities");
}
