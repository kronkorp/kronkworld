/*
** FREE PROJECT, 2026
** KRONKWORLD
** File description:
** Entity (manager)
*/
#ifndef _KRONKWORLD_ENTITY_H
    #define _KRONKWORLD_ENTITY_H
    #include "EntityError.hpp"
    #include <bitset>
    #include <cstdint>
    #include <queue>
    #include <vector>
    #define MAX_COMPONENTS 256
    // NOTE: no more hard entity cap - m_signatures/m_sparse grow on demand.
    // This is only a reserve() hint to avoid reallocations early on.
    #define ENTITY_INITIAL_CAPACITY 4096

namespace kw
{

    using Entity = uint64_t;
    using Signature = std::bitset<MAX_COMPONENTS>;

    class EntityManager
    {

    public:
        EntityManager()
        {
            m_signatures.reserve(ENTITY_INITIAL_CAPACITY);
        }

        Entity create()
        {
            Entity e;

            if (!m_availables.empty()) {
                e = m_availables.front();
                m_availables.pop();
            } else {
                e = m_id++;
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
            grow(entity);
            m_signatures[entity] = signature;
        }

        Signature& signature(
            Entity entity
        )
        {
            grow(entity);
            return m_signatures[entity];
        }

        const Signature& signature(
            Entity entity
        ) const
        {
            if (entity >= m_signatures.size()) {
                throw MaxEntitiesReached();
            }
            return m_signatures[entity];
        }

    private:
        void grow(Entity entity)
        {
            if (entity >= m_signatures.size()) {
                m_signatures.resize(entity + 1);
            }
        }

        Entity                 m_id = 0;
        std::queue<Entity>     m_availables;
        std::vector<Signature> m_signatures;
    };

}

#endif /* _KRONKWORLD_ENTITY_H */
