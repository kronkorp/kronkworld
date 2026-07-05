/*
** FREE PROJECT, 2026
** KRONKWORLD
** File description:
** World (mediator ?)
*/
#ifndef _KRONKWORLD_ENTITY_H
    #define _KRONKWORLD_ENTITY_H
    #include "EntityError.hpp"
#include <array>
    #include <bitset>
    #include <cstdint>
    #include <queue>
    #include <vector>
    #define MAX_COMPONENTS 100
    #define MAX_ENTITIES   100

namespace kw
{

    using Entity = uint64_t ;
    using Signature = std::bitset<MAX_COMPONENTS>;

    class EntityManager
    {

    public:
        Entity create()
        {
            static Entity id = 0;
            Entity e;

            if (!m_availables.empty()) {
                e = m_availables.front();
                m_availables.pop();
            } else {
                e = id++;
            }
            signature(e, 0);
            return e;
        };

        void destroy(
            Entity entity
        )
        {
            signature(entity, 0);
            m_availables.push(entity);
        }

        void signature(
            Entity entity,
            Signature signature
        )
        {
            if (entity >= m_signatures.size()) {
                throw MaxEntitiesReached();
            }
            m_signatures[entity] = signature;
        }

        Signature signature(
            Entity entity
        )
        {
            if (entity >= m_signatures.size()) {
                throw MaxEntitiesReached();
            }
            return m_signatures[entity];
        }

    private:
        std::queue<Entity>                  m_availables;
        std::array<Signature, MAX_ENTITIES> m_signatures;
    };

}

#endif /* _KRONKWORLD_ENTITY_H */
