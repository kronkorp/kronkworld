/*
** FREE PROJECT, 2026
** KRONKWORLD
** File description:
** World (mediator ?)
*/
#ifndef _KRONKWORLD_VIEW_HPP
    #define _KRONKWORLD_VIEW_HPP
    #include "../entity/Entity.hpp"
    #include "../component/Component.hpp"
    #include "../system/System.hpp"
#include "kronkworld/entity/EntityError.hpp"
    #include <cstddef>
    #include <iostream>
    #include <tuple>
    #include <utility>

namespace kw
{

    template<typename ...C>
    class View
    {
    public:

        ///////////////////////////////////////////////////////////////////////
        /**
         * @class ViewIterator
         *
         * @brief Iterator to get entities that match View's requests
         */
        ///////////////////////////////////////////////////////////////////////
        class ViewIterator
        {
            public:
            ViewIterator(
                size_t index,
                const std::vector<Entity>& leaderEntities,
                EntityManager& em,
                ComponentManager& cm,
                std::bitset<MAX_COMPONENTS> mask
            ) : m_index(index), m_leaderEntities(leaderEntities), m_em(em), m_cm(cm) , m_mask(mask)
            {
                next();
            }

            Entity operator*() const { return m_leaderEntities[m_index]; }
            // std::tuple<Entity, C&...> operator*() const
            // {
            //     auto e = m_leaderEntities[m_index];
            //     return std::forward_as_tuple(e, m_cm.get<C>(e)...);
            // }

            ViewIterator& operator++() {
                m_index++;
                next();
                return *this;
            }

            bool operator!=(const ViewIterator& other) const { return m_index != other.m_index; }

        private:
            size_t m_index;
            const std::vector<Entity>&  m_leaderEntities;
            EntityManager&              m_em;
            ComponentManager&           m_cm;
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
        ///////////////////////////////////////////////////////////////////////

        View(
            ComponentManager& cmanager,
            EntityManager&    emanager
        ) : m_cmanager(cmanager), m_emanager(emanager)
        {
            // NOTE: Expand for each C
            (m_signature.set(cmanager.id<C>()), ...);
            try {
                std::pair<IComponentBox*, size_t> boxes[] = {
                    { &cmanager.box<C>(), cmanager.box<C>().entities().size() }...
                };
                auto best_box = std::min_element(std::begin(boxes), std::end(boxes), 
                    [](const auto& a, const auto& b) {
                        return a.second < b.second;
                    }
                );
                if (!best_box || best_box->first == nullptr) {
                    throw BadEntity("feur martin");
                }
                m_best = best_box->first;
            } catch (const BadComponent&) {
                m_best = nullptr;
            }
        }

        ViewIterator begin()
        {
            // using Min = typename std::tuple_element<0, std::tuple<C...>>::type;
            // auto& box = m_cmanager.box<Min>();
            // auto& box = std::get<m_best>(std::make_tuple<IComponentBox&>(m_cmanager.box<C>()...));
            if (!m_best) return ViewIterator(-1UL, std::vector<Entity>(), m_emanager, m_cmanager, m_signature);
            auto& box = *m_best;

            return ViewIterator(0, box.entities(), m_emanager, m_cmanager, m_signature);
        }

        ViewIterator end()
        {
            // using Min = typename std::tuple_element<0, std::tuple<C...>>::type;
            // auto& box = m_cmanager.box<Min>();
            // auto& box = std::get<m_best>(std::make_tuple(m_cmanager.box<C>()...));
            if (!m_best) return ViewIterator(-1UL, std::vector<Entity>(), m_emanager, m_cmanager, m_signature);
            auto& box = *m_best;

            return ViewIterator(box.entities().size(), box.entities(), m_emanager, m_cmanager, m_signature);
        }

        template<typename Component>
        Component& get(
            Entity e
        )
        {
            return m_cmanager.get<Component>(e);
        }

        template<typename F>
        void foreach(F handler)
        {
            // NOTE: *this uses end() and begin() to get iterator and travel as fast as possible into the smallest common box...
            for (auto e : *this) {
                handler(e, m_cmanager.get<C>(e)...);
            }
        }

        Entity first()
        {
            // Hack as fuck
            for (auto e : *this) {
                return e;
            }
            throw BadEntity("No entities");
        }

    private:
        ComponentManager& m_cmanager;
        EntityManager&    m_emanager;
        Signature         m_signature;
        // size_t            m_best = 0;
        IComponentBox*    m_best;
    };

}

#endif /* _KRONKWORLD_VIEW_HPP */
