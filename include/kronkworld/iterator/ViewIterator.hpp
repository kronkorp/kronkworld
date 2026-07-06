/*
** FREE PROJECT, 2026
** KRONKWORLD
** File description:
** World (mediator ?)
*/
#ifndef _KRONKWORLD_VIEW_ITERATOR_HPP
    #define _KRONKWORLD_VIEW_ITERATOR_HPP
    #include "../entity/Entity.hpp"
    #include "../component/Component.hpp"
    #include "../system/System.hpp"
    #include <iostream>
    #include <utility>

namespace kw
{

    class ViewIterator
    {
    public:
        ViewIterator(
            size_t index,
            const std::vector<Entity>& leaderEntities,
            EntityManager& em,
            std::bitset<MAX_COMPONENTS> mask
        ) : m_index(index), m_leaderEntities(leaderEntities), m_em(em), m_mask(mask) 
        {
            next();
        }

        Entity operator*() const { return m_leaderEntities[m_index]; }

        ViewIterator& operator++() {
            m_index++;
            next();
            return *this;
        }

        bool operator!=(const ViewIterator& other) const { return m_index != other.m_index; }

    private:
        size_t m_index;
        const std::vector<Entity>& m_leaderEntities;
        EntityManager& m_em;
        std::bitset<MAX_COMPONENTS> m_mask;

        void next() {
            while (m_index < m_leaderEntities.size()) {
                Entity e = m_leaderEntities[m_index];
                if ((m_em.signature(e) & m_mask) == m_mask) {
                    break;
                }
                m_index++;
            }
        }
    };

}

#endif /* _KRONKWORLD_VIEW_ITERATOR_HPP */
