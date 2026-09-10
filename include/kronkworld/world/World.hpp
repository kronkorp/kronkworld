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
    #include "../ressource/RessourceManager.hpp"
    #include "View.hpp"
    #include "kronkworld/system/ISystem.hpp"
    #include <cstddef>
    #include <memory>
    #include <utility>

namespace kw
{

    class World
    {
    public:
        void show(Entity entity) const;
        void runOnce(void);
        void run(void);
        void stop(void);

        Entity create()
        {
            return m_entityManager.create();
        }
        
        void remove(Entity entity)
        {
            m_componentManager.clear(entity);
            m_entityManager.destroy(entity);
        }

        // template<typename ...C>
        // void spawn(C&&... components)
        // {
        //     auto e = m_entityManager.create();
        //     (this->add<C>(e), ...);
        // }

        ///////////////////////////////////////////////////////////////////////
        template<typename C, typename ...Args>
        C& add(Entity entity, Args&&... args)
        {
            m_entityManager.signature(entity).set(m_componentManager.id<C>());
            return m_componentManager.add<C>(entity, std::forward<Args>(args)...);
        }

        template<typename C>
        C& get(Entity entity)
        {
            return m_componentManager.get<C>(entity);
        }
        
        template<typename C>
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
        // World& addRender(std::unique_ptr<ISystem> system)
        // {
        //     m_systemManager.addRender(std::move(system));
        //     return *this;
        // }

        // World& addUpdate(std::unique_ptr<ISystem> system)
        // {
        //     m_systemManager.addUpdate(std::move(system));
        //     return *this;
        // }

        World& addSystem(
            size_t                   priority,
            std::unique_ptr<ISystem> system,
            size_t                   delay    = 1,
            size_t                   interval = 1,
            const RWMask&            mask     = RWMask(0, 0)
        )
        {
            m_systemManager.addSystem(
                priority,
                std::move(system),
                delay,
                interval,
                mask
            );
            return *this;
        }

        ///////////////////////////////////////////////////////////////////////
        template<typename R, typename ...Args>
        R& addResource(Args&&... args)
        {
            return m_resourceManager.put<R>(std::forward<Args>(args)...);
        }

        template<typename R>
        R& getResource(void)
        {
            return m_resourceManager.get<R>();
        }

        template<typename R>
        void removeResource(void)
        {
            m_resourceManager.remove<R>();
        }

        ///////////////////////////////////////////////////////////////////////
        template<typename ...C>
        View<C...> view(void)
        {
            return View<C...>(m_componentManager, m_entityManager);
        }


    private:
        EntityManager    m_entityManager;
        ComponentManager m_componentManager;
        SystemManager    m_systemManager;
        ResourceManager  m_resourceManager;
        bool             m_running = true;
    };

}

#endif /* _KRONKWORLD_WORLD_H */
