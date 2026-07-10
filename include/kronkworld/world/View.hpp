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
    #include <cstddef>
    #include <iostream>
    #include <tuple>
    #include <utility>
    #include "../iterator/ViewIterator.hpp"

namespace kw
{

    template<typename ...C>
    class View
    {
    public:
        View(
            ComponentManager& cmanager,
            EntityManager&    emanager
        ) : m_cmanager(cmanager), m_emanager(emanager)
        {
            // NOTE: Expand for each C
            (m_signature.set(cmanager.id<C>()), ...);
            std::pair<IComponentBox*, size_t> boxes[] = {
                { &cmanager.box<C>(), cmanager.box<C>().entities().size() }...
            };
            auto best_box = std::min_element(std::begin(boxes), std::end(boxes), 
                [](const auto& a, const auto& b) {
                    return a.second < b.second;
                }
            );
            m_best = best_box->first;
        }

        ViewIterator begin()
        {
            // using Min = typename std::tuple_element<m_best, std::tuple<C...>>::type;
            // auto& box = m_cmanager.box<Min>();
            // auto& box = std::get<m_best>(std::make_tuple<IComponentBox&>(m_cmanager.box<C>()...));
            auto& box = *m_best;

            return ViewIterator(0, box.entities(), m_emanager, m_signature);
        }

        ViewIterator end()
        {
            // using Min = typename std::tuple_element<m_best, std::tuple<C...>>::type;
            // auto& box = m_cmanager.box<Min>();
            // auto& box = std::get<m_best>(std::make_tuple(m_cmanager.box<C>()...));
            auto& box = *m_best;

            return ViewIterator(box.entities().size(), box.entities(), m_emanager, m_signature);
        }

        template<typename Component>
        Component& get(
            Entity e
        )
        {
            return m_cmanager.get<Component>(e);
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
