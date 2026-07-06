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
            // NOTE: Expand for each C in template 
            (m_signature.set(cmanager.id<C>()), ...);
        }

        ViewIterator begin()
        {
            using Min = typename std::tuple_element<0, std::tuple<C...>>::type;
            auto& box = m_cmanager.box<Min>();

            return ViewIterator(0, box.entities(), m_emanager, m_signature);
        }

        ViewIterator end()
        {
            using Min = typename std::tuple_element<0, std::tuple<C...>>::type;
            auto& box = m_cmanager.box<Min>();

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
    };

}

#endif /* _KRONKWORLD_VIEW_HPP */
