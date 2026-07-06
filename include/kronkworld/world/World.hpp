/*
** FREE PROJECT, 2026
** KRONKWORLD
** File description:
** World (mediator ?)
*/
#ifndef _KRONKWORLD_WORLD_H
    #define _KRONKWORLD_WORLD_H
    #include "../entity/Entity.hpp"
    #include "../component/Component.hpp"
    #include <cstdint>
    #include <iostream>
#include <utility>

namespace kw
{

    class world
    {
    public:
        Entity create()
        {
            return m_entityManager.create();
        }

        void remove(Entity entity)
        {
            m_entityManager.destroy(entity);
        }

        template<typename C, typename ...Args>
        Component add(Entity entity, Args&&... args)
        {
            return m_componentManager.add<C>(entity, std::forward<Args>(args)...);
        }

        template<typename C>
        C& get(Entity entity)
        {
            return m_componentManager.get<C>(entity);
        }

        template<typename C, typename ...Args>
        void remove(Entity entity)
        {
            m_componentManager.remove<C>(entity);
        }

        void show(Entity entity) const
        {
            std::cout << "Entity : " <<  entity << std::endl;
        }

    private:
        EntityManager    m_entityManager;
        ComponentManager m_componentManager;
    };

}

#endif /* _KRONKWORLD_WORLD_H */
