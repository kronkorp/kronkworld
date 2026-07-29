#include <chrono>
#include <iostream>
#include <locale>
#include <memory>
#include "../../../include/kronkworld/Kronkworld.hpp"
#include <format>

struct Dt
{
    float val;
};

class TimeSystem : public kw::ISystem
{
public:
    void handle(kw::World& world) override
    {
        auto ctime = std::chrono::high_resolution_clock::now();

        std::chrono::duration<float> frameTime = ctime - m_lastTime;
        m_lastTime = ctime;

        auto& dt = world.getResource<Dt>();
        dt.val = frameTime.count();
        std::cout << "dt is : " << dt.val << " s" << std::endl;
    }

    private:
        std::chrono::high_resolution_clock::time_point m_lastTime = std::chrono::high_resolution_clock::now();
};

int main(
    int argc,
    char *const *argv
)
{
    kw::World world;

    world.addResource<Dt>();
    world.addUpdate(std::make_unique<TimeSystem>());
    while (1) {
        world.runOnce();
    }
    return 0;
}
