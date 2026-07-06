/*
** FREE PROJECT, 2026
** KRONKWORLD
** File description:
** Entity (manager)
*/
#ifndef _KRONKWORLD_COMPONENT_H
    #define _KRONKWORLD_COMPONENT_H
    #include "ComponentBox.hpp"
#include "ComponentError.hpp"
#include <array>
    #include <cstdint>
#include <exception>
#include <memory>
#include <utility>

namespace kw
{
    typedef uint64_t Component;

    class ComponentManager
    {
    public:

        template<typename C>
        C& get(Entity e)
        {
            auto idx = id<C>();

            if (idx >= MAX_COMPONENTS || m_componentBoxs[idx] == nullptr) {
                throw MaxComponentReached();
            }
            return static_cast<ComponentBox<C>*>(m_componentBoxs[idx].get())->get(e);
        }

        template<typename C, typename ...Args>
        Component add(Entity e, Args&&... args)
        {
            auto idx = id<C>();

            if (idx >= MAX_COMPONENTS) {
                throw MaxComponentReached();
            }
            if (m_componentBoxs[idx] == nullptr) {
                m_componentBoxs[idx] = std::make_unique<ComponentBox<C>>();
            }
            auto& box = *static_cast<ComponentBox<C>*>(m_componentBoxs[idx].get());
            box.add(e, std::forward<Args>(args)...);
            return idx;
        }

        template<typename C>
        void remove(Entity e)
        {
            auto idx = id<C>();

            if (idx >= MAX_COMPONENTS || m_componentBoxs[idx] == nullptr) {
                throw MaxComponentReached();
            }
            auto& box = *static_cast<ComponentBox<C>*>(m_componentBoxs[idx].get());
            box.remove(e);
        }

        template<typename C>
        Component id(void) const
        {
            static Component id = m_id++;
            return id;
        }

    private:
        inline static Component m_id;
        std::array<std::unique_ptr<IComponentBox>, MAX_COMPONENTS> m_componentBoxs;
    };

}

#endif /* _KRONKWORLD_COMPONENT_H */
