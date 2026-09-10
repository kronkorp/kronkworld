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
    extern "C" {
        #include "kronkflow/task.h"
    }
    #include "kronkworld/kronkflow/Scheduler.hpp"

namespace kw
{

    typedef          uint32_t StageId;

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
        SystemManager() : m_scheduler(128) {}

        // void addUpdate(std::unique_ptr<ISystem> system)
        // {
        //     m_logicSystems.push_back(std::move(system));
        // }

        // void addRender(std::unique_ptr<ISystem> system)
        // {
        //     m_renderSystems.push_back(std::move(system));
        // }

        void addSystem(
            StageId                  stage,
            std::unique_ptr<ISystem> system,
            size_t                   delay    = 1,
            size_t                   interval = 0,
            const RWMask&            mask     = RWMask(0, 0)
        )
        {
            m_systems.push_back(std::move(system));
            m_scheduler.pushTask((kfTaskOpt){
                [](void *ctx, void *arg) -> int {
                    auto task = static_cast<ISystem *>(arg);
                    auto ret = task->handle(*static_cast<World *>(ctx));
                    task->markAsDone();
                    return ret;
                },
                static_cast<void *>(m_systems.back().get()),
                NULL,
                stage,
                (kfRWMasks){mask.read_mask, mask.write_mask}},
            delay, interval);
        }

        void runOnce(World& world)
        {
            // for (auto& ls : m_logicSystems) {
            //     ls->handle(world);
            // }
            // for (auto& rs : m_renderSystems) {
            //     rs->handle(world);
            // }
            m_scheduler.tick(static_cast<void *>(&world));
        }

    private:

        // std::vector<std::unique_ptr<ISystem>> m_logicSystems;
        // std::vector<std::unique_ptr<ISystem>> m_renderSystems;

        std::vector<std::unique_ptr<ISystem>> m_systems;
        Scheduler                             m_scheduler;
    };

}

#endif /* _KRONKWORLD_SYSTEM_H */
