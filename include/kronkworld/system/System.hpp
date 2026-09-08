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
    #include <utility>
    #include <vector>
    #include "ISystem.hpp"
    #include "kronkflow/macros/types.h"
    extern "C" {
        #include "kronkflow/task.h"
    }
    #include "kronkworld/kronkflow/Scheduler.hpp"

namespace kw
{

    typedef          uint32_t StageId;
    static constexpr StageId  Startup = 0;

    typedef size_t RunPolicy;
    static constexpr RunPolicy EachFrame = 1;

    struct RWMask {

        RWMask(uint64_t read, uint64_t write) : read_mask(read), write_mask(write) {}

        uint64_t read_mask;
        uint64_t write_mask;

    };

    class SystemManager
    {
    public:
        SystemManager() : m_scheduler(128) {}

        void addUpdate(std::unique_ptr<ISystem> system)
        {
            m_logicSystems.push_back(std::move(system));
        }

        void addRender(std::unique_ptr<ISystem> system)
        {
            m_renderSystems.push_back(std::move(system));
        }

        void addSystem(
            [[maybe_unused]] StageId       stage,
            std::unique_ptr<ISystem>       system,
            [[maybe_unused]] const RWMask& mask   = RWMask(0, 0),
            [[maybe_unused]] RunPolicy     policy = EachFrame,
            [[maybe_unused]] size_t        delay  = 1
        )
        {
            auto interval = 1;

            m_systems.push_back(std::move(system));
            m_scheduler.pushTask((kfTaskOpt){
                [](void *ctx, void *arg) -> int {
                    auto task = static_cast<ISystem *>(arg);
                    auto ret = task->handle(*static_cast<World *>(ctx));
                    task->markAsDone();
                    return ret;
                },
                static_cast<void *>(m_systems.back().get()), NULL},
            delay, interval);
        }

        void runOnce(World& world)
        {
            // FIXME: Will remove this two for loops.
            for (auto& ls : m_logicSystems) {
                ls->handle(world);
            }
            for (auto& rs : m_renderSystems) {
                rs->handle(world);
            }
            m_scheduler.tick(static_cast<void *>(&world));
            // for (auto& s : m_systems) {
            //     if (s->isDone()) {

            //     }
            // }
        }

    private:

        std::vector<std::unique_ptr<ISystem>> m_logicSystems;
        std::vector<std::unique_ptr<ISystem>> m_renderSystems;

        std::vector<std::unique_ptr<ISystem>> m_systems;
        Scheduler                             m_scheduler;
    };

}

#endif /* _KRONKWORLD_SYSTEM_H */
