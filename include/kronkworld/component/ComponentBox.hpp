/*
** FREE PROJECT, 2026
** KRONKWORLD
** File description:
** Entity (manager)
*/
#ifndef _KRONKWORLD_BOX_H
    #define _KRONKWORLD_BOX_H
    #include <algorithm>
    #include <array>
    #include <cstdint>
#include <exception>
    #include <utility>
    #include <vector>
    #include "../entity/Entity.hpp"
#include "ComponentError.hpp"

namespace kw
{

    class IComponentBox
    {
    public:
        virtual ~IComponentBox() = default;
    };

    template <typename C>
    class ComponentBox : public IComponentBox 
    {
    public:
        ComponentBox()
        {
            m_sparse.fill(-1UL);
        }

        C& get(Entity entity)
        {
            if (entity >= MAX_ENTITIES) {
                throw MaxEntitiesReached();
            }
            return m_raw[m_sparse[entity]];
        }

        template <typename ...Args>
        C& add(Entity entity, Args&&... args)
        {
            if (entity >= MAX_ENTITIES) {
                throw MaxEntitiesReached();
            }
            auto idx = m_sparse[entity];
            if (idx == -1UL) {
                m_raw.emplace_back(std::forward<Args>(args)...);
                m_reverse.push_back(entity);
                m_sparse[entity] = m_raw.size() - 1;
                return m_raw.back();
            }
            m_raw[idx] = C(std::forward<Args>(args)...);
            return m_raw[idx];
        }

        void remove(Entity entity)
        {
            auto idx = m_sparse[entity];
            if (idx == -1UL) {
                throw MaxEntitiesReached();
            }
            auto backIdx = m_reverse.size() - 1;
            auto backEntt = m_reverse[backIdx];
            if (backIdx != idx) {
                m_raw[idx] = std::move(m_raw[backIdx]);
                m_reverse[idx] = backEntt;
                m_sparse[backEntt] = idx;
            }
            m_raw.pop_back();
            m_reverse.pop_back();
            m_sparse[entity] = -1UL;
        }

    private:
        std::array<Entity, MAX_ENTITIES> m_sparse;
        std::vector<C>                   m_raw;
        std::vector<Entity>              m_reverse;
    };

}

#endif /* _KRONKWORLD_BOX_H */
