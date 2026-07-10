extern "C" {
    #include "kronklab/kronklab.h"
}
#include "../../include/kronkworld/Kronkworld.hpp"
#include <cstddef>
#include <string.h>

struct Conf {

    std::string name;

};

Test(resource, basic_resource)
{
    kw::World world;

    world.addResource<Conf>("World");
    AssertStrEq(world.getResource<Conf>().name.c_str(), "World", "Should be world, is %s", world.getResource<Conf>().name.c_str());
}

Test(resource, resource_removing)
{
    kw::World world;
    bool dummyboool = false;

    world.addResource<Conf>("World");
    world.removeResource<Conf>();
    try {
        world.getResource<Conf>();
    } catch (const kw::ResourceError& e) {
        dummyboool = true;
    }
    AssertEq(dummyboool, true, "Should throw");
}

Test(resource, resource_replacing)
{
    kw::World world;

    world.addResource<Conf>("World");
    // world.removeResource<Conf>();
    world.addResource<Conf>("Victor");
    AssertStrEq(world.getResource<Conf>().name.c_str(), "Victor", "Should be equal to victor");
}
