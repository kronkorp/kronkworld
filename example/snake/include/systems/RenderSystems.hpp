#include "kronkworld/system/ISystem.hpp"
#include "kronkworld/world/World.hpp"

#pragma once

class WindowRenderSystem : public kw::ISystem
{
    public:
        bool handle(kw::World& world) override;
};

class HUDRenderSystem : public kw::ISystem
{
    public:
        bool handle(kw::World& world) override;
};

class WindowDisplaySystem : public kw::ISystem
{
    public:
        bool handle(kw::World& world) override;
};

class WindowClearSystem : public kw::ISystem
{
    public:
        bool handle(kw::World& world) override;
};

class TextsRenderSystem : public kw::ISystem
{
    public:
        bool handle(kw::World& world) override;
};
