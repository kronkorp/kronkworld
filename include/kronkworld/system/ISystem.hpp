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

        virtual void handle(World& world) = 0;
    };

}

#endif /* _KRONKWORLD_ILOGICSYSTEM_HPP */
