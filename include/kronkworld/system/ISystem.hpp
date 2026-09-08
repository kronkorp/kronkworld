/*
** FREE PROJECT, 2026
** KRONKWORLD
** File description:
** World (mediator ?)
*/
#ifndef _KRONKWORLD_ISYSTEM_HPP
    #define _KRONKWORLD_ISYSTEM_HPP

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

}

#endif /* _KRONKWORLD_ISYSTEM_HPP */
