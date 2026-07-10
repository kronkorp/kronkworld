#include <memory>
extern "C" {
    #include "kronklab/kronklab.h"
}
#include "../../include/kronkworld/Kronkworld.hpp"
#include <cstddef>

struct s1 {};
struct s2 {};
struct s3 {};

class DummySystem : public kw::ISystem
{
    public:
        void handle(kw::World& world) override
        {
            auto view = world.view<s1>();

            for ([[maybe_unused]] auto e : view) {
                ++m_dummy_ctr;
            }
        }

        size_t get() const noexcept {
            return m_dummy_ctr;
        }

    private:
        size_t m_dummy_ctr = 0;
};

class DummySystem2 : public kw::ISystem
{
    public:
        void handle(kw::World& world) override
        {
            auto view = world.view<s1, s3>();

            for ([[maybe_unused]] auto e : view) {
                ++m_dummy_ctr;
            }
        }

        size_t get() const noexcept {
            return m_dummy_ctr;
        }

    private:
        size_t m_dummy_ctr = 0;
};

class DummySystem3 : public kw::ISystem
{
    public:
        void handle(kw::World& world) override
        {
            auto view = world.view<s1, s2, s3>();

            for ([[maybe_unused]] auto e : view) {
                ++m_dummy_ctr;
            }
        }

        size_t get() const noexcept {
            return m_dummy_ctr;
        }

    private:
        size_t m_dummy_ctr = 0;
};

Test(systems, adding_system)
{
    kw::World world;

    for (size_t i = 0; i < 200; ++i) {
        auto e = world.create();
        if (i & 1) {
            world.add<s1>(e);
        }
    }
    DummySystem sys;
    sys.handle(world);
    AssertEq(sys.get(), 100, "Should touch 100 entities");
}

// FIXME: Throw and catch when box<C> does not exists...
Test(systems, adding_complex_system)
{
    kw::World world;

    for (size_t i = 0; i < 200; ++i) {
        auto e = world.create();
        if (!(i & 1)) {
            world.add<s1>(e);
        }
        if (!(i % 10)) {
            world.add<s3>(e);
        }
    }
    DummySystem2 sys;
    sys.handle(world);
    AssertEq(sys.get(), 20, "Should touch 20 entities but touched : %zu", sys.get());
}

Test(systems, adding_system_with_0_view)
{
    kw::World world;

    for (size_t i = 0; i < 200; ++i) {
        auto e = world.create();
        if (i & 1) {
            world.add<s1>(e);
        }
    }
    DummySystem3 sys;
    sys.handle(world);
    AssertEq(sys.get(), 0, "Should touch 0 entities");
}
