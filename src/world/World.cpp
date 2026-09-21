#include "kronkworld/world/World.hpp"

void kw::World::show(Entity entity) const
{
    std::cout << "Entity : " <<  entity << std::endl;
}

void kw::World::runOnce(void)
{
    runOnce(Schedule::Fixed);
    runOnce(Schedule::Frame);
}

void kw::World::runOnce(Schedule schedule)
{
    m_systemManager.runOnce(*this, schedule);
}

void kw::World::run(void)
{
    while (m_running) {
        runOnce();
    }
}

void kw::World::stop(void)
{
    m_running = false;
}
