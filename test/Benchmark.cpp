/*
** FREE PROJECT, 2026
** KRONKWORLD
** File description:
** High-Performance View Benchmark
*/
#include "../include/kronkworld/Kronkworld.hpp"
#include <chrono>
#include <iostream>
#include <memory>
#include <vector>
#include <numeric>

struct Position {
    float x;
    float y;
};

struct Velocity {
    float v;
};

volatile float g_dummy_accumulator = 0.0f;

void run_dense_benchmark(size_t entity_count)
{
    auto world = std::make_unique<kw::World>();

    std::cout << "--- 1. BENCHMARK DENSE (" << entity_count << " entites) ---" << std::endl;
    std::cout << "Allocation en cours..." << std::endl;
    
    for (size_t i = 0; i < entity_count; ++i) {
        kw::Entity e = world->create();
        world->add<Position>(e, i * 1.0f, i * 2.0f);
        world->add<Velocity>(e, 0.5f);
    }

    std::cout << "Execution de l'iteration sur la View..." << std::endl;
    auto start = std::chrono::high_resolution_clock::now();

    auto view = world->view<Position, Velocity>();
    for (auto e : view) {
        auto& pos = world->get<Position>(e);
        auto& vel = world->get<Velocity>(e);
        
        pos.x += vel.v;
        g_dummy_accumulator += pos.x; 
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    std::cout << "Temps d'execution : " << duration << " us (" << duration / 1000.0f << " ms)" << std::endl << std::endl;
}

void run_sparse_benchmark(size_t total_entities, size_t target_entities)
{
    auto world = std::make_unique<kw::World>();

    std::cout << "--- 2. BENCHMARK SPARSE (Aiguille dans la botte de foin) ---" << std::endl;
    std::cout << "Creation de " << total_entities << " Positions et " << target_entities << " Velocities..." << std::endl;

    for (size_t i = 0; i < total_entities; ++i) {
        kw::Entity e = world->create();
        world->add<Position>(e, 1.0f, 1.0f);

        if (i % (total_entities / target_entities) == 0) {
            world->add<Velocity>(e, 2.0f);
        }
    }

    auto start = std::chrono::high_resolution_clock::now();

    auto view = world->view<Velocity, Position>();
    size_t iterated_count = 0;
    
    for (auto e : view) {
        auto& pos = world->get<Position>(e);
        auto& vel = world->get<Velocity>(e);
        
        pos.x += vel.v;
        g_dummy_accumulator += pos.x;
        iterated_count++;
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    
    std::cout << "Entites iterees valides : " << iterated_count << " / " << target_entities << std::endl;
    std::cout << "Temps d'execution : " << duration << " us (" << duration / 1000.0f << " ms)" << std::endl << std::endl;
}

void run_fragmented_benchmark(size_t entity_count)
{
    auto world = std::make_unique<kw::World>();
    std::vector<kw::Entity> entities;
    entities.reserve(entity_count);

    std::cout << "--- 3. BENCHMARK FRAGMENTATION (Swap & Pop Stress Test) ---" << std::endl;

    for (size_t i = 0; i < entity_count; ++i) {
        kw::Entity e = world->create();
        world->add<Position>(e, 1.0f, 1.0f);
        world->add<Velocity>(e, 1.0f);
        entities.push_back(e);
    }
    std::cout << "Fragmentation de la memoire (destruction de 50% des entites)..." << std::endl;
    for (size_t i = 0; i < entities.size(); i += 2) {
        world->remove(entities[i]);
    }

    auto start = std::chrono::high_resolution_clock::now();

    auto view = world->view<Position, Velocity>();
    for (auto e : view) {
        auto& pos = world->get<Position>(e);
        g_dummy_accumulator += pos.x;
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    std::cout << "Temps d'execution apres fragmentation : " << duration << " us (" << duration / 1000.0f << " ms)" << std::endl << std::endl;
}

int main(void)
{
    std::cout << "=========================================" << std::endl;
    std::cout << "        KRONKWORLD VIEW BENCHMARK        " << std::endl;
    std::cout << "=========================================" << std::endl << std::endl;

    run_dense_benchmark(100000);
    run_sparse_benchmark(100000, 1000);
    run_fragmented_benchmark(100000);

    return 0;
}
