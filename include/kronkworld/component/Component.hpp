/*
** FREE PROJECT, 2026
** KRONKWORLD
** File description:
** Entity (manager)
*/
#ifndef _KRONKWORLD_COMPONENT_H
    #define _KRONKWORLD_COMPONENT_H
    #include <array>
    #include <cstdint>

namespace kw
{
    typedef uint64_t Component;

    class ComponentManager
    {
    public:

        template<typename C>
        void Add()
        {
            return;
        }

        template<typename C>
        Component id(void)
        {
            static Component id = m_id++;
            return id;
        }

    private:
        Component m_id;
        // std::array<ComponentBox, MAX_COMPONENTS> m_componentBoxs;
    };

}

#endif /* _KRONKWORLD_COMPONENT_H */
