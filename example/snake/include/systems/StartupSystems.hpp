#include "kronkworld/system/ISystem.hpp"
#include "kronkworld/world/World.hpp"

#pragma once

class ResourcesStartupSystem : public kw::ISystem
{
    public:
        bool handle(kw::World& world) override;
};

class SnakeStartupSystem : public kw::ISystem
{
    public:
        bool handle(kw::World& world) override;
};

class AppleStartupSystem : public kw::ISystem
{
    public:
        bool handle(kw::World& world) override;
};
