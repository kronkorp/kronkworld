/*
** FREE PROJECT, 2026
** KRONKWORLD
** File description:
** World (mediator ?)
*/
#ifndef _KRONKWORLD_SYSTEM_H
    #define _KRONKWORLD_SYSTEM_H
    #include <cstddef>
    #include <cstdint>
    #include <memory>
    #include <new>
    #include <utility>
    #include <vector>
    #include "ISystem.hpp"
    extern "C" {
        #include "kronkflow/task.h"
    }
    #include "kronkworld/kronkflow/Scheduler.hpp"

namespace kw
{

    typedef          uint32_t StageId;

    // A World owns two schedulers, each one with its own tick counter (so
    // "delay" and "interval" count ticks of the schedule the system is in):
    //  - Fixed: run as many times as needed to catch up with a fixed timestep
    //  - Frame: run once per rendered frame
    // kw doesn't decide when they run: the caller does, with runOnce(schedule).
    enum class Schedule : uint8_t { Fixed, Frame };

    // Identifies a system that was added, to remove it later
    struct SystemHandle {

        Schedule schedule = Schedule::Fixed;
        kfTaskID id       = 0;   // 0: no system

        explicit operator bool() const { return id != 0; }

    };

    // static constexpr StageId  Startup = 0;

    // typedef size_t RunPolicy;
    // static constexpr RunPolicy EachFrame = 1;

    struct RWMask {

        RWMask() : read_mask(0), write_mask(0) {}
        RWMask(uint64_t read, uint64_t write) : read_mask(read), write_mask(write) {}

        uint64_t read_mask;
        uint64_t write_mask;

    };

    class SystemManager
    {
    public:
        SystemManager() : m_fixed(128), m_frame(128) {}

        // void addUpdate(std::unique_ptr<ISystem> system)
        // {
        //     m_logicSystems.push_back(std::move(system));
        // }

        // void addRender(std::unique_ptr<ISystem> system)
        // {
        //     m_renderSystems.push_back(std::move(system));
        // }

        // Systems of a same stage, due on the same tick, run in the order
        // they were added. A system that returns false is not run again.
        SystemHandle addSystem(
            Schedule                 schedule,
            StageId                  stage,
            std::unique_ptr<ISystem> system,
            size_t                   delay    = 1,
            size_t                   interval = 1,
            const RWMask&            mask     = RWMask(0, 0)
        )
        {
            ISystem* rawSystem = system.release();

            auto id = scheduler(schedule).pushTask((kfTaskOpt){
                [](void *ctx, void *arg) -> int {
                    auto task = static_cast<ISystem *>(arg);
                    auto ret = task->handle(*static_cast<World *>(ctx));
                    task->markAsDone();
                    return ret;
                },
                static_cast<void *>(rawSystem),
                [](void *thing){ delete static_cast<ISystem *>(thing); },
                stage,
                (kfRWMasks){mask.read_mask, mask.write_mask}},
            delay, interval);
            if (id == 0) {
                delete rawSystem;
                throw std::bad_alloc();
            }
            return SystemHandle{schedule, id};
        }

        // Same as above, in the Fixed schedule
        SystemHandle addSystem(
            StageId                  stage,
            std::unique_ptr<ISystem> system,
            size_t                   delay    = 1,
            size_t                   interval = 1,
            const RWMask&            mask     = RWMask(0, 0)
        )
        {
            return addSystem(Schedule::Fixed, stage, std::move(system), delay, interval, mask);
        }

        // Can be called from a system, including on itself (it then finishes
        // its current run and is not run again).
        // Returns false if it already ended, or if the handle is empty.
        bool removeSystem(const SystemHandle& handle)
        {
            return handle && scheduler(handle.schedule).remove(handle.id);
        }

        void runOnce(World& world, Schedule schedule)
        {
            scheduler(schedule).tick(static_cast<void *>(&world));
        }

    private:

        Scheduler& scheduler(Schedule schedule)
        {
            return schedule == Schedule::Fixed ? m_fixed : m_frame;
        }

        Scheduler m_fixed;
        Scheduler m_frame;
    };

}

#endif /* _KRONKWORLD_SYSTEM_H */
