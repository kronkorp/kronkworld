#include <chrono>
#include <cstddef>
#include "../../../../include/kronkworld/Kronkworld.hpp"
#include "kronkworld/system/ISystem.hpp"
#include "kronkworld/world/World.hpp"

#pragma once

enum Stages : std::size_t {

    Startup,
    ScanEvents,
    PreUpdate,
    Update,
    PostUpdate,
    PreRender,
    Render,
    PostRender

};

class StartupSystem : public kw::ISystem
{
    public:
        bool handle(kw::World& world) override;
};

class WindowEventSystem : public kw::ISystem
{
    public:
        bool handle(kw::World& world) override;
};

class TimeSystem : public kw::ISystem
{
    public:
        bool handle(kw::World& world) override;

    private:
        std::chrono::high_resolution_clock::time_point m_lastTime = std::chrono::high_resolution_clock::now();
};

class WindowRenderSystem : public kw::ISystem
{
    public:
        bool handle(kw::World& world) override;
};

class MovementUpdateSystem : public kw::ISystem
{
    public:
        bool handle(kw::World& world) override;
};

class PlayerInputApplySystem : public kw::ISystem
{
    public:
        bool handle(kw::World& world) override;
};

class AppleCollisionSystem : public kw::ISystem
{
    public:
        bool handle(kw::World& world) override;
};
