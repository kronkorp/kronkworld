/*
** FREE PROJECT, 2026
** KRONKWORLD
** File description:
** World (mediator ?)
*/
#ifndef _KRONKWORLD_SYSTEM_H
    #define _KRONKWORLD_SYSTEM_H
    #include <cstddef>
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

        void addSystem([[maybe_unused]] size_t priority, std::unique_ptr<ISystem> system)
        {
            m_systems.push_back(std::move(system));
            // FIXME: Can't call null aha dumbass
            m_scheduler.pushTask((kfTaskOpt){
                [](void *ctx, void *arg) -> int {
                    auto task = static_cast<ISystem *>(arg);
                    return task->handle(*static_cast<World *>(ctx));
                },
                static_cast<void *>(m_systems.back().get()), NULL},
            1, 1);
        }

        void runOnce(World& world)
        {
            for (auto& ls : m_logicSystems) {
                ls->handle(world);
            }
            for (auto& rs : m_renderSystems) {
                rs->handle(world);
            }
            m_scheduler.tick(static_cast<void *>(&world));
        }

    private:

        std::vector<std::unique_ptr<ISystem>> m_logicSystems;
        std::vector<std::unique_ptr<ISystem>> m_renderSystems;

        std::vector<std::unique_ptr<ISystem>> m_systems;
        Scheduler                             m_scheduler;
    };

}

#endif /* _KRONKWORLD_SYSTEM_H */
