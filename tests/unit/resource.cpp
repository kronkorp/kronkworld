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
