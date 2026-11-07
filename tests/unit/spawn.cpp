extern "C" {
    #include "kronklab/kronklab.h"
}
#include "kronkworld/Kronkworld.hpp"
#include <cstddef>
#include <stdexcept>

namespace {

    struct SpPos { float x; float y; };
    struct SpVel { float dx; float dy; };
    struct SpTag {};

    // Throws when it is moved into a box, to make World::spawn fail midway
    struct SpBoom {
        SpBoom() = default;
        SpBoom(SpBoom&&) { throw std::runtime_error("boom"); }
        SpBoom& operator=(SpBoom&&) = default;
    };

    size_t count(kw::World& world)
    {
        size_t n = 0;

        for (auto e : world.view<SpPos>()) {
            (void)e;
            ++n;
        }
        return n;
    }

}

Test(spawn, with_components)
{
    kw::World world;

    auto e = world.spawn(SpPos{1.f, 2.f}, SpVel{3.f, 4.f});
    AssertEq(world.get<SpPos>(e).x, 1.f, "Should be 1 but is %f", world.get<SpPos>(e).x);
    AssertEq(world.get<SpPos>(e).y, 2.f, "Should be 2 but is %f", world.get<SpPos>(e).y);
    AssertEq(world.get<SpVel>(e).dx, 3.f, "Should be 3 but is %f", world.get<SpVel>(e).dx);
    AssertEq(world.get<SpVel>(e).dy, 4.f, "Should be 4 but is %f", world.get<SpVel>(e).dy);
    AssertEq(world.has<SpPos>(e), true, "Should own SpPos");
    AssertEq(world.has<SpVel>(e), true, "Should own SpVel");
}

Test(spawn, empty)
{
    kw::World world;

    auto a = world.spawn();
    auto b = world.spawn();
    AssertEq(a, 0, "First entity should be 0 but is %zu", (size_t)a);
    AssertEq(b, 1, "Second entity should be 1 but is %zu", (size_t)b);
    AssertEq(world.has<SpPos>(a), false, "Should own nothing");
    world.add<SpPos>(a, 7.f, 8.f);
    AssertEq(world.get<SpPos>(a).x, 7.f, "Should still accept add<>");
}

Test(spawn, tag_component)
{
    kw::World world;

    auto e = world.spawn(SpTag{});
    AssertEq(world.has<SpTag>(e), true, "Should own the tag");
}

Test(spawn, lvalue_is_copied)
{
    kw::World world;
    SpPos p{5.f, 6.f};

    auto e = world.spawn(p);
    p.x = 9.f;
    AssertEq(world.get<SpPos>(e).x, 5.f, "Should be a copy, got %f", world.get<SpPos>(e).x);
}

Test(spawn, seen_by_views)
{
    kw::World world;

    for (size_t i = 0; i < 3; ++i) {
        world.spawn(SpPos{1.f, 1.f}, SpVel{0.f, 0.f});
    }
    world.spawn(SpVel{0.f, 0.f});
    size_t n = count(world);
    AssertEq(n, 3, "View<SpPos> should see 3 entities, got %zu", n);
}

Test(spawn, mixed_then_remove)
{
    kw::World world;
    bool threw = false;

    auto a = world.spawn(SpPos{1.f, 1.f});
    auto b = world.spawn(SpVel{2.f, 2.f}, SpTag{});
    try {
        world.remove(a);
        world.remove(b);
    } catch (const std::exception&) {
        threw = true;
    }
    AssertEq(threw, false, "Spawned entities of different shapes must be removable");
}

Test(spawn, throw_destroys_entity)
{
    kw::World world;
    bool threw = false;

    world.spawn(SpPos{0.f, 0.f});
    try {
        world.spawn(SpPos{1.f, 1.f}, SpBoom{});
    } catch (const std::runtime_error&) {
        threw = true;
    }
    AssertEq(threw, true, "spawn should rethrow");
    size_t n = count(world);
    AssertEq(n, 1, "Half-built entity should be gone, view sees %zu", n);
    auto e = world.create();
    AssertEq(e, 1, "Failed entity id should be recycled, got %zu", (size_t)e);
    AssertEq(world.has<SpBoom>(e), false, "Recycled entity must be clean");
}
