/*
** FREE PROJECT, 2026
** KRONKWORLD
** File description:
** World (mediator ?)
*/
#ifndef _KRONKWORLD_ILOGICSYSTEM_HPP
    #define _KRONKWORLD_ILOGICSYSTEM_HPP
    #include "../entity/Entity.hpp"
    #include "../component/Component.hpp"
    #include <iostream>
    #include <utility>
    #include <vector>

namespace kw
{

    class World;

    class ISystem
    {
    public:
        virtual ~ISystem() = default;

        // NOTE: Handle returns a bool to stop the scheduler to reinserting if needed
        virtual bool handle(World& world) = 0;

        void markAsDone(bool done = true) { m_isDone = done; }
        bool isDone(void) const { return m_isDone; }

    private:
            bool m_isDone;
    };

    // class IRenderSystem : public ISystem
    // {
    // public:
    //     virtual ~IRenderSystem() = default;
    // };

    // class ILogicSystem : public ISystem
    // {
    //     public:
    //         virtual ~ILogicSystem() = default;
    // };

}

#endif /* _KRONKWORLD_ILOGICSYSTEM_HPP */
