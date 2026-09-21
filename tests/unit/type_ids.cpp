extern "C" {
    #include "kronklab/kronklab.h"
}
#include "../../include/kronkworld/Kronkworld.hpp"
#include <array>
#include <cstddef>
#include <set>
#include <thread>
#include <utility>
#include <vector>

namespace {

    struct WorldA { int v; };
    struct WorldB { int v; };

    template<int N> struct Tag { int v; };

    // Calls f.template operator()<N>() for N = 0..Count-1 (or the reverse)
    template<typename F, int... Ns>
    void forEachTag(F&& f, std::integer_sequence<int, Ns...>, bool reverse)
    {
        constexpr int last = static_cast<int>(sizeof...(Ns)) - 1;

        if (!reverse) {
            (f.template operator()<Ns>(), ...);
        } else {
            (f.template operator()<last - Ns>(), ...);
        }
    }

    constexpr int TAGS = 64;

    // Every thread registers the same TAGS types, half of them in reverse order,
    // then all of them must agree on the id of each type, and ids are distinct.
    template<typename IdOf>
    void checkIdsAcrossThreads(IdOf idOf)
    {
        constexpr int THREADS = 8;
        std::vector<std::array<size_t, TAGS>> ids(THREADS);
        std::vector<std::thread> threads;

        for (int t = 0; t < THREADS; ++t) {
            threads.emplace_back([&, t] {
                forEachTag([&]<int N>() { ids[t][N] = idOf.template operator()<N>(); },
                    std::make_integer_sequence<int, TAGS>{}, t % 2 == 1);
            });
        }
        for (auto& th : threads) {
            th.join();
        }
        std::set<size_t> distinct(ids[0].begin(), ids[0].end());
        AssertEq(distinct.size(), TAGS, "ids must be distinct, got %zu distinct", distinct.size());
        for (int t = 1; t < THREADS; ++t) {
            AssertEq(ids[t] == ids[0], true, "thread %d disagrees on the ids", t);
        }
    }

}

// Regression: the id used to come from a counter of the first manager that
// saw the type, so a second World could give two types the same slot.
Test(type_ids, resources_independent_of_world)
{
    kw::World w1;
    kw::World w2;

    w1.addResource<WorldA>(1);
    w2.addResource<WorldB>(20);
    w2.addResource<WorldA>(10);
    AssertEq(w2.getResource<WorldB>().v, 20, "B untouched by A in w2, got %d", w2.getResource<WorldB>().v);
    AssertEq(w2.getResource<WorldA>().v, 10, "A in w2, got %d", w2.getResource<WorldA>().v);
    AssertEq(w1.getResource<WorldA>().v, 1, "A in w1, got %d", w1.getResource<WorldA>().v);
}

Test(type_ids, resource_ids_thread_safe)
{
    checkIdsAcrossThreads([]<int N>() { return kw::ResourceManager::id<Tag<N>>(); });
}

Test(type_ids, component_ids_thread_safe)
{
    kw::ComponentManager components;

    checkIdsAcrossThreads([&]<int N>() { return static_cast<size_t>(components.id<Tag<N>>()); });
}
