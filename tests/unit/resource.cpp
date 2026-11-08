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

namespace {

    int g_order = 0;
    int g_systemDied = 0;
    int g_resourceDied = 0;

    struct Probe {
        ~Probe() { g_resourceDied = ++g_order; }
    };

    struct ProbeSystem : kw::ISystem {
        ~ProbeSystem() override { g_systemDied = ++g_order; }
        bool handle(kw::World&) override { return true; }
    };

}

Test(resource, systems_die_before_resources)
{
    {
        kw::World world;

        world.addResource<Probe>();
        world.addSystem(kw::Schedule::Fixed, 0, std::make_unique<ProbeSystem>());
    }
    AssertEq(g_systemDied, 1, "The system should be destroyed first");
    AssertEq(g_resourceDied, 2, "The resource should be destroyed after the system");
}
