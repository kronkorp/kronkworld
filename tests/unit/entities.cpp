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

Test(entities, creation_scales_past_old_cap)
{
    kw::World world;
    bool threw = false;

    // NOTE: entity storage used to be a fixed 256-slot array; this creates
    // well past that former cap to confirm it now grows instead of throwing.
    try {
        for (size_t i = 0; i < 100000; ++i) {
            world.create();
        }
    } catch (const kw::EntityError& e) {
        threw = true;
    }
    AssertEq(threw, false, "Should not throw: entity storage grows on demand");
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
