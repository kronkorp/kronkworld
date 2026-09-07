#include "kronkworld/world/World.hpp"

void kw::World::show(Entity entity) const
{
    std::cout << "Entity : " <<  entity << std::endl;
}

void kw::World::runOnce(void)
{
    m_systemManager.runOnce(*this);
}

void kw::World::run(void)
{
    while (m_running) {
        m_systemManager.runOnce(*this);
    }
}

void kw::World::stop(void)
{
    m_running = false;
}
