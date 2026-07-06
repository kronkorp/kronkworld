/*
** FREE PROJECT, 2026
** KRONKWORLD
** File description:
** World (mediator ?)
*/
#ifndef _KRONKWORLD_SYSTEM_H
    #define _KRONKWORLD_SYSTEM_H
    #include "../entity/Entity.hpp"
    #include "../component/Component.hpp"
    #include <iostream>
    #include <memory>
    #include <utility>
    #include <vector>
    #include "ISystem.hpp"

namespace kw
{

    class SystemManager
    {
    public:
        void addUpdate(std::unique_ptr<ISystem> system)
        {
            m_logicSystems.push_back(std::move(system));
        }

        void addRender(std::unique_ptr<ISystem> system)
        {
            m_renderSystems.push_back(std::move(system));
        }

        void runOnce(World& world)
        {
            for (auto& ls : m_logicSystems) {
                ls->handle(world);
            }
            for (auto& rs : m_renderSystems) {
                rs->handle(world);
            }
        }

    private:
        std::vector<std::unique_ptr<ISystem>> m_logicSystems;
        std::vector<std::unique_ptr<ISystem>> m_renderSystems;
    };

}

#endif /* _KRONKWORLD_SYSTEM_H */
