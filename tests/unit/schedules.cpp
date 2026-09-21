extern "C" {
    #include "kronklab/kronklab.h"
}
#include "../../include/kronkworld/Kronkworld.hpp"
#include <cstddef>
#include <memory>
#include <utility>
#include <vector>

namespace {

    // Counts its runs, and how many instances were destroyed (= cleared)
    class Counter : public kw::ISystem
    {
        public:
            explicit Counter(size_t* runs, size_t* destroyed = nullptr)
                : m_runs(runs), m_destroyed(destroyed) {}

            ~Counter() override
            {
                if (m_destroyed) {
                    ++*m_destroyed;
                }
            }

            bool handle(kw::World&) override
            {
                ++*m_runs;
                return true;
            }

        private:
            size_t* m_runs;
            size_t* m_destroyed;
    };

    class Recorder : public kw::ISystem
    {
        public:
            Recorder(std::vector<int>* log, int marker) : m_log(log), m_marker(marker) {}

            bool handle(kw::World&) override
            {
                m_log->push_back(m_marker);
                return true;
            }

        private:
            std::vector<int>* m_log;
            int               m_marker;
    };

    // Removes itself the first time it runs
    class SelfRemover : public kw::ISystem
    {
        public:
            SelfRemover(size_t* runs, kw::SystemHandle* handle) : m_runs(runs), m_handle(handle) {}

            bool handle(kw::World& world) override
            {
                ++*m_runs;
                world.removeSystem(*m_handle);
                return true;
            }

        private:
            size_t*           m_runs;
            kw::SystemHandle* m_handle;
    };

}

Test(schedules, fixed_and_frame_are_independent)
{
    kw::World world;
    size_t fixed = 0, frame = 0;

    world.addSystem(kw::Schedule::Fixed, 0, std::make_unique<Counter>(&fixed));
    world.addSystem(kw::Schedule::Frame, 0, std::make_unique<Counter>(&frame));
    for (int i = 0; i < 3; ++i) {
        world.runOnce(kw::Schedule::Fixed);
    }
    AssertEq(fixed, 3, "3 fixed ticks, got %zu", fixed);
    AssertEq(frame, 0, "no frame tick yet, got %zu", frame);
    for (int i = 0; i < 2; ++i) {
        world.runOnce(kw::Schedule::Frame);
    }
    AssertEq(fixed, 3, "frame ticks don't run fixed systems, got %zu", fixed);
    AssertEq(frame, 2, "2 frame ticks, got %zu", frame);
    world.runOnce();
    AssertEq(fixed, 4, "runOnce() ticks both, fixed is %zu", fixed);
    AssertEq(frame, 3, "runOnce() ticks both, frame is %zu", frame);
}

Test(schedules, legacy_add_system_goes_in_fixed)
{
    kw::World world;
    size_t runs = 0;

    world.addSystem(0, std::make_unique<Counter>(&runs));
    for (int i = 0; i < 5; ++i) {
        world.runOnce(kw::Schedule::Frame);
    }
    AssertEq(runs, 0, "not in the frame schedule, got %zu", runs);
    world.runOnce(kw::Schedule::Fixed);
    AssertEq(runs, 1, "in the fixed schedule, got %zu", runs);
}

Test(schedules, manager_default_periodic)
{
    kw::World world;
    kw::SystemManager manager;
    size_t runs = 0;

    manager.addSystem(0, std::make_unique<Counter>(&runs));
    for (int i = 0; i < 4; ++i) {
        manager.runOnce(world, kw::Schedule::Fixed);
    }
    AssertEq(runs, 4, "same default as World (periodic), got %zu", runs);
}

Test(schedules, one_shot_system_runs_once)
{
    kw::World world;
    size_t runs = 0, destroyed = 0;

    world.addSystem(0, std::make_unique<Counter>(&runs, &destroyed), 0, 0);
    for (int i = 0; i < 4; ++i) {
        world.runOnce();
    }
    AssertEq(runs, 1, "interval 0 runs once, got %zu", runs);
    AssertEq(destroyed, 1, "and is destroyed, got %zu", destroyed);
}

Test(schedules, remove_system)
{
    kw::World world;
    size_t runs = 0, destroyed = 0;
    auto handle = world.scheduleSystem(kw::Schedule::Fixed, 0, std::make_unique<Counter>(&runs, &destroyed));

    AssertEq(static_cast<bool>(handle), true, "a valid handle");
    world.runOnce();
    world.runOnce();
    AssertEq(runs, 2, "ran twice, got %zu", runs);
    AssertEq(world.removeSystem(handle), true, "removed");
    AssertEq(destroyed, 1, "system destroyed on removal, got %zu", destroyed);
    world.runOnce();
    world.runOnce();
    AssertEq(runs, 2, "no more runs, got %zu", runs);
    AssertEq(world.removeSystem(handle), false, "already removed");
    AssertEq(world.removeSystem(kw::SystemHandle{}), false, "empty handle");
}

Test(schedules, remove_in_right_schedule)
{
    kw::World world;
    size_t fixed = 0, frame = 0;
    auto h1 = world.scheduleSystem(kw::Schedule::Fixed, 0, std::make_unique<Counter>(&fixed));
    auto h2 = world.scheduleSystem(kw::Schedule::Frame, 0, std::make_unique<Counter>(&frame));

    // The two schedulers hand out ids on their own: a handle needs its schedule
    AssertEq(h1.id, h2.id, "same first id in each scheduler");
    world.removeSystem(h2);
    world.runOnce();
    AssertEq(fixed, 1, "fixed system untouched, got %zu", fixed);
    AssertEq(frame, 0, "frame system removed, got %zu", frame);
}

Test(schedules, system_removes_itself)
{
    kw::World world;
    size_t runs = 0;
    kw::SystemHandle handle;

    handle = world.scheduleSystem(kw::Schedule::Fixed, 0, std::make_unique<SelfRemover>(&runs, &handle));
    for (int i = 0; i < 5; ++i) {
        world.runOnce();
    }
    AssertEq(runs, 1, "ran once then gone, got %zu", runs);
}

Test(schedules, registration_order)
{
    kw::World world;
    std::vector<int> log;

    for (int m = 1; m <= 6; ++m) {
        world.addSystem(kw::Schedule::Fixed, 2, std::make_unique<Recorder>(&log, m));
    }
    world.addSystem(kw::Schedule::Fixed, 1, std::make_unique<Recorder>(&log, 0));
    for (int t = 0; t < 3; ++t) {
        world.runOnce(kw::Schedule::Fixed);
    }
    AssertEq(log.size(), 21, "7 systems x 3 ticks, got %zu", log.size());
    for (size_t i = 0; i < log.size(); ++i) {
        int expected = (i % 7 == 0) ? 0 : static_cast<int>(i % 7);
        AssertEq(log[i], expected, "at %zu expected %d got %d", i, expected, log[i]);
    }
}
