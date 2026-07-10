/*
** FREE PROJECT, 2026
** KRONKWORLD
** File description:
** World (mediator ?)
*/
#ifndef _KRONKWORLD_RESOURCE_MANAGER_H
    #define _KRONKWORLD_RESOURCE_MANAGER_H
    #include "../entity/Entity.hpp"
    #include "../component/Component.hpp"
    #include <array>
    #include <cstddef>
    #include <memory>
    #include <utility>
    #include "IResource.hpp"
    #include "Resource.hpp"
    #include "ResourceError.hpp"
    #define MAX_RESOURCES 256

namespace kw
{

    class ResourceManager
    {
    public:
        template<typename R>
        size_t id(void) noexcept
        {
            static size_t id = m_id++;
            return id;
        }

        template<typename R, typename ...Args>
        R& put(Args&&... args)
        {
            auto id = this->id<R>();

            if (id >= MAX_RESOURCES) {
                throw MaxResourceReached();
            }
            this->m_resources[id] = std::make_shared<R>(std::forward<Args>(args)...);
            return *static_cast<Resource<R>*>(this->m_resources[id].get());
        }

        template<typename R>
        R& get(void)
        {
            auto id = this->id<R>();

            if (id >= MAX_RESOURCES) {
                throw MaxResourceReached();
            }
            if (m_resources[id] == nullptr) {
                throw BadResource("Resource \"{}\" does not exists", id);
            }
            return *static_cast<Resource<R>*>(this->m_resources[id].get());
        }

        template<typename R>
        void remove(void)
        {
            auto id = this->id<R>();

            if (id >= MAX_RESOURCES) {
                throw MaxResourceReached();
            }
            this->m_resources[id] = nullptr;
        }

    private:
        std::array<std::unique_ptr<IResource>, MAX_RESOURCES> m_resources;
        size_t                               m_id;
    };

}

#endif /* _KRONKWORLD_RESOURCE_MANAGER_H */
