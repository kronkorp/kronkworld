/*
** FREE PROJECT, 2026
** KRONKWORLD
** File description:
** IResource
*/
#ifndef _KRONKWORLD_IRESOURCE_H
    #define _KRONKWORLD_IRESOURCE_H
    #include "../entity/Entity.hpp"
    #include "../component/Component.hpp"
    #include <array>
    #include <iostream>
    #include <memory>
    #include <utility>
    #include <vector>

namespace kw
{

    class IResource
    {
        public:
            virtual ~IResource() = default;
    };

}

#endif /* _KRONKWORLD_IRESOURCE_H */
