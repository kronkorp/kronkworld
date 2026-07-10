extern "C" {
    #include "kronklab/kronklab.h"
}
#include "../../include/kronkworld/Kronkworld.hpp"
#include <cstddef>

struct Position {

    float x;
    float y;

};

Test(components, creation)
{
    kw::World world;

    auto e = world.create();
    world.add<Position>(e, 100.f, 30.f);
    auto& p = world.get<Position>(e);
    AssertEq(p.x, 100.f, "Should be equal to 100 but is %f", p.x);
}

Test(components, deletation)
{
    kw::World world;
    bool dummybool = false;

    auto e = world.create();
    world.add<Position>(e, 100.f, 30.f);
    auto& p = world.get<Position>(e);
    AssertEq(p.x, 100.f, "Should be equal to 100 but is %f", p.x);
    try {
        world.remove<Position>(e);
        world.get<Position>(e);
    } catch (const kw::EntityError& e) {
        dummybool = true;
    }
    AssertEq(dummybool, true, "Should throw on bad component");
}
