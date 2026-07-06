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
    #include "../system/System.hpp"
    #include <iostream>
    #include <utility>

namespace kw
{

    class World
    {
    public:
        Entity create()
        {
            return m_entityManager.create();
        }
        
        void remove(Entity entity)
        {
            m_componentManager.clear(entity);
            m_entityManager.destroy(entity);
        }

        ///////////////////////////////////////////////////////////////////////
        template<typename C, typename ...Args>
        Component add(Entity entity, Args&&... args)
        {
            m_entityManager.signature(entity).set(m_componentManager.id<C>());
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
            m_entityManager.signature(entity).set(m_componentManager.id<C>(), false);
            m_componentManager.remove<C>(entity);
        }

        template<typename C>
        bool has(Entity entity) const
        {
            return m_entityManager.signature(entity).test(m_componentManager.id<C>());
        }

        ///////////////////////////////////////////////////////////////////////
        void addRender(std::unique_ptr<ISystem> system)
        {
            m_systemManager.addRender(std::move(system));
        }

        void addUpdate(std::unique_ptr<ISystem> system)
        {
            m_systemManager.addUpdate(std::move(system));
        }

        ///////////////////////////////////////////////////////////////////////
        void show(Entity entity) const
        {
            std::cout << "Entity : " <<  entity << std::endl;
        }

        void runOnce()
        {
            m_systemManager.runOnce(*this);
        }

    private:
        // TODO: Ressources

        EntityManager    m_entityManager;
        ComponentManager m_componentManager;
        SystemManager    m_systemManager;
    };

}

#endif /* _KRONKWORLD_WORLD_H */
