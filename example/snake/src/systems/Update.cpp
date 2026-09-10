#include <SFML/Graphics/Rect.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <format>
#include "components/Components.hpp"
#include "kronkworld/entity/Entity.hpp"
#include "kronkworld/world/World.hpp"
#include "systems/UpdateSystems.hpp"
#include "resources/Resources.hpp"

bool WindowEventSystem::handle(kw::World& world)
{
    auto& win = world.getResource<Window>();
    sf::Event evt;
    auto e = world.view<PlayerInput>().first();

    while (win.window.pollEvent(evt)) {
        if (evt.type == sf::Event::Closed) {
            world.stop();
            return false;
        }
        if (evt.type == sf::Event::KeyPressed) {
            switch (evt.key.code) {
                case sf::Keyboard::Key::Up:
                    world.get<PlayerInput>(e).direction = Directions::Up; break;
                case sf::Keyboard::Key::Down:
                    world.get<PlayerInput>(e).direction = Directions::Down; break;
                case sf::Keyboard::Key::Right:
                    world.get<PlayerInput>(e).direction = Directions::Right; break;
                case sf::Keyboard::Key::Left:
                    world.get<PlayerInput>(e).direction = Directions::Left; break;
                default:
                    continue;
            }
        }
    }
    return true;
}

bool TimeSystem::handle(kw::World& world)
{
    auto now = std::chrono::high_resolution_clock::now();
    std::chrono::duration<float> frameTime = now - m_lastTime;
    m_lastTime = now;

    auto& dt = world.getResource<Dt>();
    dt.val = frameTime.count();

    m_accumulator += dt.val;
    m_frameCount++;

    if (m_accumulator >= 1.0f) {
        auto& fps = world.getResource<FPS>();
        fps.fps = static_cast<float>(m_frameCount) / m_accumulator;

        m_accumulator = 0.f;
        m_frameCount = 0;
    }
    return true;
}

bool PlayerInputApplySystem::handle(kw::World& world)
{
    auto view = world.view<PlayerInput, Velocity>();

    view.foreach([](kw::Entity, PlayerInput& input, Velocity& vel){
        switch (input.direction) {
            case Directions::Up:
                vel.x = 0;
                vel.y = -1;
                break;
            case Directions::Down:
                vel.x = 0;
                vel.y = 1;
                break;
            case Directions::Left:
                vel.x = -1;
                vel.y = 0;
                break;
            case Directions::Right:
                vel.x = 1;
                vel.y = 0;
                break;
        }
    });
    return true;
}

bool MovementUpdateSystem::handle(kw::World& world)
{
    auto& dt = world.getResource<Dt>();
    m_accumulator += dt.val;

    if (m_accumulator < m_stepInterval) {
        return true;
    }

    m_accumulator = 0.f;

    auto view = world.view<Body, Velocity>();

    view.foreach([](kw::Entity, Body& body, Velocity& vel){
        body.rect.move(vel.x * step, vel.y * step);
    });

    return true;
}

bool AppleCollisionSystem::handle(kw::World& world)
{
    auto apple = world.view<Body, Apple>().first();
    auto head = world.view<Body, SnakeHead>().first();
    auto& win = world.getResource<Window>();
    auto& score = world.getResource<Score>();

    if (world.get<Body>(apple).rect.getGlobalBounds().intersects(world.get<Body>(head).rect.getGlobalBounds())) {
        world.get<Body>(apple).rect.setPosition(
            (rand() % (win.window.getSize().x / 20)) * 20.f,
            (rand() % (win.window.getSize().y / 20)) * 20.f
        );

        ++score.score;

        auto& h = world.get<SnakeHead>(head);
        sf::Vector2f spawnPos = h.history.empty() ? world.get<Body>(head).rect.getPosition() : h.history.back();

        auto body = world.create();
        auto& bodyComp = world.add<Body>(body, sf::Vector2f{20.f, 20.f});
        bodyComp.rect.setPosition(spawnPos);

        std::size_t tailCount = world.view<SnakeBody>().size();
        world.add<SnakeBody>(body, tailCount + 1);
    }

    return true;
}

bool TailMovementSystem::handle(kw::World& world)
{
    auto headView = world.view<SnakeHead, Body>();

    auto headEntity = headView.first();
    auto& head = world.get<SnakeHead>(headEntity);
    auto& headBody = world.get<Body>(headEntity);

    if (head.history.front() == headBody.rect.getPosition()) {
        return true;
    }
    head.history.push_front(headBody.rect.getPosition());

    auto bodyView = world.view<SnakeBody, Body>();

    size_t s = 0;
    bodyView.foreach([&head, &s](kw::Entity, SnakeBody& index, Body& body){
        std::size_t targetIdx = index.idx - 1;
        s++;
        if (targetIdx < head.history.size()) {
            body.rect.setPosition(head.history[targetIdx]);
        }
    });

    std::size_t totalTailSegments = s;
    while (head.history.size() > totalTailSegments) {
        head.history.pop_back();
    }

    return true;
}
