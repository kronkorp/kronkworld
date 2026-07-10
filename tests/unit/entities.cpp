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
