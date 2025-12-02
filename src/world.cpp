#include "world.hpp"
#include "statistics.hpp"
#include "neural_network.hpp"
#include <random>
#include <cmath>
#include <algorithm>
#include <chrono>

World::World(const SimulationConfig& cfg, unsigned seed) {
    config = const_cast<SimulationConfig*>(&cfg);
    boundary = cfg.boundary;
    grid = std::make_unique<SpatialGrid>(boundary, cfg.grid_cells);

    std::mt19937 rng(seed);
    std::uniform_real_distribution<double> dist(-1.0, 1.0);
    std::bernoulli_distribution predatorChance(cfg.predator_chance);

    agents.resize(cfg.num_agents);
    for (size_t i = 0; i < cfg.num_agents; ++i) {
        auto& a = agents[i];
        a.pos = {dist(rng) * boundary, dist(rng) * boundary};
        a.vel = {dist(rng), dist(rng)};
        a.predator = predatorChance(rng);
        a.energy = cfg.initial_energy;
        a.alive = true;
        a.generation = 0;
        
        // Initialize neural network
        if (cfg.enable_ai) {
            initialize_brain(a, seed + i);
        }
    }
}

void World::update(double dt) {
    // Apply AI control if enabled
    if (config && config->enable_ai) {
        apply_neural_control(dt);
    }
    
    handle_interactions(dt);
    integrate_movement(dt);
    handle_energy_and_reproduction(dt);
    remove_dead_agents();
    
    // Update fitness and age
    for (auto& a : agents) {
        if (a.alive) {
            a.age++;
            a.fitness = a.age * 0.1 + a.energy * 0.5 + a.kills * 10.0;
        }
    }
}

void World::handle_interactions(double dt) {
    if (!config) return;

    // Rebuild spatial grid
    grid->clear();
    for (size_t i = 0; i < agents.size(); ++i) {
        if (agents[i].alive) {
            grid->insert(i, agents[i].pos);
        }
    }

    // Process interactions using spatial grid
    for (size_t i = 0; i < agents.size(); ++i) {
        auto& a = agents[i];
        if (!a.alive) continue;

        grid->query_radius(a.pos, std::sqrt(config->interaction_range), [&](size_t j) {
            if (i == j) return;
            auto& b = agents[j];
            if (!b.alive) return;

            double dx = b.pos.x - a.pos.x;
            double dy = b.pos.y - a.pos.y;
            double dist2 = dx*dx + dy*dy + 1e-6;

            // Predator chases prey (only if AI is disabled)
            if (!config->enable_ai) {
                if (a.predator && !b.predator && dist2 < config->interaction_range) {
                    a.vel.x += config->predator_chase_strength * dx;
                    a.vel.y += config->predator_chase_strength * dy;
                } 
                // Prey flees from predator
                else if (!a.predator && b.predator && dist2 < config->interaction_range) {
                    a.vel.x -= config->prey_flee_strength * dx;
                    a.vel.y -= config->prey_flee_strength * dy;
                }
            }
            
            // Eating mechanics (always active)
            if (a.predator && !b.predator && dist2 < config->eating_range) {
                b.alive = false;
                a.energy += config->energy_gain_from_prey;
                if (a.energy > config->max_energy) a.energy = config->max_energy;
                a.kills++;
                if (stats) stats->record_death();
            }
            
            // Separation (avoid crowding)
            if (dist2 < config->separation_range) {
                a.vel.x -= config->separation_strength * dx;
                a.vel.y -= config->separation_strength * dy;
            }
        });
    }
}

void World::integrate_movement(double dt) {
    for (auto& a : agents) {
        if (!a.alive) continue;

        // Update trail
        if (config && config->show_trails) {
            a.trail.push_back(a.pos);
            if (a.trail.size() > static_cast<size_t>(config->trail_length)) {
                a.trail.pop_front();
            }
        } else {
            a.trail.clear();
        }

        a.pos.x += a.vel.x * dt;
        a.pos.y += a.vel.y * dt;
        
        // Wall bounce
        if (a.pos.x > boundary || a.pos.x < -boundary) a.vel.x *= -1;
        if (a.pos.y > boundary || a.pos.y < -boundary) a.vel.y *= -1;
        
        // Clamp position
        a.pos.x = std::clamp(a.pos.x, -boundary, boundary);
        a.pos.y = std::clamp(a.pos.y, -boundary, boundary);
    }
}

void World::handle_energy_and_reproduction(double dt) {
    if (!config) return;

    std::vector<Agent> new_agents;

    for (auto& a : agents) {
        if (!a.alive) continue;

        // Consume energy
        a.energy -= config->energy_consumption_rate * dt;

        // Death from starvation
        if (a.energy <= 0.0) {
            a.alive = false;
            if (stats) stats->record_death();
            continue;
        }

        // Reproduction
        if (a.energy >= config->reproduction_energy_threshold) {
            a.energy -= config->reproduction_energy_cost;
            
            Agent offspring;
            offspring.pos = a.pos;
            offspring.vel = {a.vel.x * 0.9, a.vel.y * 0.9};
            offspring.predator = a.predator;
            offspring.energy = config->reproduction_energy_cost * 0.5;
            offspring.alive = true;
            offspring.generation = a.generation + 1;
            
            // Inherit and mutate brain
            if (config->enable_ai && a.brain) {
                offspring.brain = std::make_unique<NeuralNetwork>(a.brain->clone());
                offspring.brain->mutate(config->mutation_rate, config->mutation_strength);
            } else if (config->enable_ai) {
                // Parent has no brain, create new one
                std::random_device rd;
                initialize_brain(offspring, rd());
            }
            
            new_agents.push_back(std::move(offspring));
            if (stats) stats->record_birth();
        }
    }

    // Add newborns to population
    for (auto& offspring : new_agents) {
        agents.push_back(std::move(offspring));
    }
}

void World::remove_dead_agents() {
    agents.erase(
        std::remove_if(agents.begin(), agents.end(),
                      [](const Agent& a) { return !a.alive; }),
        agents.end()
    );
}

int World::count_predators() const {
    return std::count_if(agents.begin(), agents.end(),
                        [](const Agent& a) { return a.predator && a.alive; });
}

int World::count_prey() const {
    return std::count_if(agents.begin(), agents.end(),
                        [](const Agent& a) { return !a.predator && a.alive; });
}

void World::spawn_prey(int count) {
    if (!config) return;
    
    std::mt19937 rng(static_cast<unsigned>(std::chrono::high_resolution_clock::now().time_since_epoch().count()));
    std::uniform_real_distribution<double> dist(-1.0, 1.0);
    
    for (int i = 0; i < count; ++i) {
        Agent a;
        a.pos = {dist(rng) * boundary, dist(rng) * boundary};
        a.vel = {dist(rng), dist(rng)};
        a.predator = false;
        a.energy = config->initial_energy;
        a.alive = true;
        a.generation = generation_counter;
        
        if (config->enable_ai) {
            initialize_brain(a, rng());
        }
        
        agents.push_back(std::move(a));
        if (stats) stats->record_birth();
    }
}

void World::spawn_predators(int count) {
    if (!config) return;
    
    std::mt19937 rng(static_cast<unsigned>(std::chrono::high_resolution_clock::now().time_since_epoch().count()));
    std::uniform_real_distribution<double> dist(-1.0, 1.0);
    
    for (int i = 0; i < count; ++i) {
        Agent a;
        a.pos = {dist(rng) * boundary, dist(rng) * boundary};
        a.vel = {dist(rng), dist(rng)};
        a.predator = true;
        a.energy = config->initial_energy;
        a.alive = true;
        a.generation = generation_counter;
        
        if (config->enable_ai) {
            initialize_brain(a, rng());
        }
        
        agents.push_back(std::move(a));
        if (stats) stats->record_birth();
    }
}

// AI Methods Implementation
void World::initialize_brain(Agent& agent, unsigned seed) {
    if (!config) return;
    agent.brain = std::make_unique<NeuralNetwork>(
        config->neural_input_size,
        config->neural_hidden_size,
        config->neural_output_size,
        seed
    );
}

std::vector<double> World::get_agent_inputs(const Agent& agent, size_t agent_idx) {
    std::vector<double> inputs(config->neural_input_size, 0.0);
    
    // Find nearest prey, predator, and any agent
    double nearest_prey_dist = 1e9;
    double nearest_predator_dist = 1e9;
    double nearest_agent_dist = 1e9;
    Vec2 nearest_prey = {0, 0};
    Vec2 nearest_predator = {0, 0};
    Vec2 nearest_agent = {0, 0};
    
    for (size_t i = 0; i < agents.size(); ++i) {
        if (i == agent_idx || !agents[i].alive) continue;
        
        const auto& other = agents[i];
        double dx = other.pos.x - agent.pos.x;
        double dy = other.pos.y - agent.pos.y;
        double dist = std::sqrt(dx*dx + dy*dy);
        
        // Track nearest prey
        if (!other.predator && dist < nearest_prey_dist) {
            nearest_prey_dist = dist;
            nearest_prey = {dx, dy};
        }
        
        // Track nearest predator
        if (other.predator && dist < nearest_predator_dist) {
            nearest_predator_dist = dist;
            nearest_predator = {dx, dy};
        }
        
        // Track nearest any agent
        if (dist < nearest_agent_dist) {
            nearest_agent_dist = dist;
            nearest_agent = {dx, dy};
        }
    }
    
    // Normalize inputs to [-1, 1] range
    double norm_factor = 1.0 / (boundary * 2.0);
    
    inputs[0] = nearest_prey.x * norm_factor;
    inputs[1] = nearest_prey.y * norm_factor;
    inputs[2] = nearest_predator.x * norm_factor;
    inputs[3] = nearest_predator.y * norm_factor;
    inputs[4] = nearest_agent.x * norm_factor;
    inputs[5] = nearest_agent.y * norm_factor;
    inputs[6] = (agent.energy / config->max_energy) * 2.0 - 1.0;  // -1 to 1
    inputs[7] = std::tanh(std::sqrt(agent.vel.x*agent.vel.x + agent.vel.y*agent.vel.y));  // velocity magnitude
    
    return inputs;
}

void World::apply_neural_control(double dt) {
    if (!config) return;
    
    for (size_t i = 0; i < agents.size(); ++i) {
        auto& a = agents[i];
        if (!a.alive || !a.brain) continue;
        
        // Get sensory inputs
        auto inputs = get_agent_inputs(a, i);
        
        // Forward pass through neural network
        auto outputs = a.brain->forward(inputs);
        
        // Apply outputs as acceleration (scaled)
        double accel_scale = 0.05;  // Control responsiveness
        a.vel.x += outputs[0] * accel_scale;
        a.vel.y += outputs[1] * accel_scale;
        
        // Limit velocity
        double max_vel = 2.0;
        double vel_mag = std::sqrt(a.vel.x*a.vel.x + a.vel.y*a.vel.y);
        if (vel_mag > max_vel) {
            a.vel.x = (a.vel.x / vel_mag) * max_vel;
            a.vel.y = (a.vel.y / vel_mag) * max_vel;
        }
    }
}

