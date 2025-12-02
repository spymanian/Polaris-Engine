#pragma once
#include <string>

struct SimulationConfig {
    // Simulation parameters
    int num_agents = 10;
    double dt = 0.1;
    int max_steps = 2000;
    unsigned seed = 42;
    double boundary = 6.0;

    // Agent parameters (rebalanced)
    double predator_chance = 0.25;
    double initial_energy = 100.0;
    double max_energy = 200.0;
    double energy_consumption_rate = 0.3;  // Reduced from 0.5
    double energy_gain_from_prey = 80.0;  // Increased from 50.0
    double reproduction_energy_threshold = 130.0;  // Reduced from 150.0
    double reproduction_energy_cost = 50.0;  // Reduced from 60.0
    
    // AI/Evolution parameters
    bool enable_ai = true;
    int neural_input_size = 8;  // [nearest_prey_dx, nearest_prey_dy, nearest_predator_dx, nearest_predator_dy, nearest_agent_dx, nearest_agent_dy, energy, velocity]
    int neural_hidden_size = 12;
    int neural_output_size = 2;  // [acceleration_x, acceleration_y]
    double mutation_rate = 0.1;
    double mutation_strength = 0.2;

    // Interaction parameters (rebalanced)
    double predator_chase_strength = 0.015;  // Reduced from 0.02
    double prey_flee_strength = 0.035;  // Increased from 0.03
    double separation_strength = 0.015;  // Reduced from 0.02
    double interaction_range = 20.0;  // Increased from 16.0
    double separation_range = 3.0;  // Reduced from 4.0
    double eating_range = 1.5;  // Increased from 1.0

    // Spatial partitioning
    int grid_cells = 20;

    // Visualization
    int window_width = 800;
    int window_height = 800;
    int render_fps = 60;
    bool show_trails = false;
    int trail_length = 20;

    // Statistics
    bool enable_stats = true;
    int stats_interval = 100;
    std::string stats_output_file = "stats.csv";

    // Load from JSON file
    bool load_from_file(const std::string& filename);
    
    // Save to JSON file
    bool save_to_file(const std::string& filename) const;

    // Create default config
    static SimulationConfig create_default();
};
