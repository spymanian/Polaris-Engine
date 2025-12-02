#pragma once
#include <vector>
#include <memory>
#include "agent.hpp"
#include "config.hpp"
#include "spatial_grid.hpp"

class Statistics;

struct World {
    std::vector<Agent> agents;
    double boundary = 6.0;
    SimulationConfig* config = nullptr;
    Statistics* stats = nullptr;
    std::unique_ptr<SpatialGrid> grid;
    int generation_counter = 0;

    World(const SimulationConfig& cfg, unsigned seed);
    void update(double dt);
    void set_statistics(Statistics* s) { stats = s; }
    
    // Spawning methods
    void spawn_prey(int count);
    void spawn_predators(int count);
    
    int count_predators() const;
    int count_prey() const;

private:
    void handle_interactions(double dt);
    void integrate_movement(double dt);
    void handle_energy_and_reproduction(double dt);
    void remove_dead_agents();
    
    // AI methods
    void apply_neural_control(double dt);
    std::vector<double> get_agent_inputs(const Agent& agent, size_t agent_idx);
    void initialize_brain(Agent& agent, unsigned seed);
};
