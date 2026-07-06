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

        template<typename C>
        Component add(Entity entity)
        {
            return m_componentManager.id<C>();
        }

    private:
        EntityManager    m_entityManager;
        ComponentManager m_componentManager;
    };

}

#endif /* _KRONKWORLD_WORLD_H */
