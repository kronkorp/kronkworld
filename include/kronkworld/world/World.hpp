/*
** FREE PROJECT, 2026
** KRONKWORLD
** File description:
** World (mediator ?)
*/
#ifndef _KRONKWORLD_WORLD_H
    #define _KRONKWORLD_WORLD_H
    #include "../entity/Entity.hpp"

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

    private:
        EntityManager m_entityManager;
    };

}

#endif /* _KRONKWORLD_WORLD_H */
