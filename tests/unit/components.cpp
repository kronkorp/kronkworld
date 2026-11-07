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

struct Tag_A { int v; };
struct Tag_B { int v; };

// Regression: World::remove(Entity) used to throw as soon as the entity
// did not own every component type registered in the world.
Test(components, remove_mixed_signatures)
{
    kw::World world;
    bool threw = false;
    bool gone = false;

    auto a = world.create();
    auto b = world.create();
    world.add<Tag_A>(a, 1);
    world.add<Tag_B>(b, 2);

    try {
        world.remove(a);
    } catch (const std::exception&) {
        threw = true;
    }
    AssertEq(threw, false, "remove(entity) must not throw when it lacks some registered components");
    AssertEq(world.get<Tag_B>(b).v, 2, "Other entities must keep their components");
    try {
        world.get<Tag_A>(a);
    } catch (const kw::EntityError&) {
        gone = true;
    }
    AssertEq(gone, true, "Removed entity must lose its component");
}
