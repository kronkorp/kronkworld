extern "C" {
    #include "kronklab/kronklab.h"
}
#include "../../include/kronkworld/Kronkworld.hpp"
#include <cstddef>

struct s1 {};
struct s2 {};

Bench(benchmarks, bench_view_1)
{
    kw::World world;

    BenchStart();
    for (size_t i = 0; i < 10000000000; ++i);
    for (size_t i = 0; i < 250; ++i) {
        auto e = world.create();
        world.add<s1>(e);
    }
    auto v = world.view<s1>();
    BenchEnd();
    BenchReport();
}
