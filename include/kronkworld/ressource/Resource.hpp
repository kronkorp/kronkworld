/*
** FREE PROJECT, 2026
** KRONKWORLD
** File description:
** IResource
*/
#ifndef _KRONKWORLD_RESOURCE_H
    #define _KRONKWORLD_RESOURCE_H
    #include "../entity/Entity.hpp"
    #include "../component/Component.hpp"
    #include "IResource.hpp"
    #include <array>
    #include <iostream>
    #include <memory>
    #include <utility>
    #include <vector>

namespace kw
{

    template<typename R>
    class Resource : public IResource
    {
        public:
            template<typename ...Args>
            Resource(Args&&... args) : m_internal({std::forward<Args>(args)...}) {}

            Resource(const Resource<R>& other) = delete;
            Resource<R>& operator=(const Resource<R>& other) = delete;

            const R& get() const noexcept { return m_internal; };
            R& get() noexcept { return m_internal; };

            template<typename ...Args>
            R& put(Args&&... args)
            {
                m_internal = R({std::forward<Args>(args)...});
                return m_internal;
            }

        private:
            R m_internal;
    };

}

#endif /* _KRONKWORLD_RESOURCE_H */
