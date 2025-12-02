#include "config.hpp"
#include "../external/json.hpp"
#include <fstream>
#include <iostream>

using json = nlohmann::json;

bool SimulationConfig::load_from_file(const std::string& filename) {
    try {
        std::ifstream file(filename);
        if (!file.is_open()) {
            std::cerr << "[Config] Failed to open file: " << filename << std::endl;
            return false;
        }

        json j = json::parse(file);

        // Load all parameters with validation
        if (j.contains("num_agents")) num_agents = j["num_agents"];
        if (j.contains("dt")) dt = j["dt"];
        if (j.contains("max_steps")) max_steps = j["max_steps"];
        if (j.contains("seed")) seed = j["seed"];
        if (j.contains("boundary")) boundary = j["boundary"];
        
        if (j.contains("predator_chance")) predator_chance = j["predator_chance"];
        if (j.contains("initial_energy")) initial_energy = j["initial_energy"];
        if (j.contains("max_energy")) max_energy = j["max_energy"];
        if (j.contains("energy_consumption_rate")) energy_consumption_rate = j["energy_consumption_rate"];
        if (j.contains("energy_gain_from_prey")) energy_gain_from_prey = j["energy_gain_from_prey"];
        if (j.contains("reproduction_energy_threshold")) reproduction_energy_threshold = j["reproduction_energy_threshold"];
        if (j.contains("reproduction_energy_cost")) reproduction_energy_cost = j["reproduction_energy_cost"];
        
        // AI parameters
        if (j.contains("enable_ai")) enable_ai = j["enable_ai"];
        if (j.contains("neural_input_size")) neural_input_size = j["neural_input_size"];
        if (j.contains("neural_hidden_size")) neural_hidden_size = j["neural_hidden_size"];
        if (j.contains("neural_output_size")) neural_output_size = j["neural_output_size"];
        if (j.contains("mutation_rate")) mutation_rate = j["mutation_rate"];
        if (j.contains("mutation_strength")) mutation_strength = j["mutation_strength"];
        
        if (j.contains("predator_chase_strength")) predator_chase_strength = j["predator_chase_strength"];
        if (j.contains("prey_flee_strength")) prey_flee_strength = j["prey_flee_strength"];
        if (j.contains("separation_strength")) separation_strength = j["separation_strength"];
        if (j.contains("interaction_range")) interaction_range = j["interaction_range"];
        if (j.contains("separation_range")) separation_range = j["separation_range"];
        if (j.contains("eating_range")) eating_range = j["eating_range"];
        
        if (j.contains("grid_cells")) grid_cells = j["grid_cells"];
        if (j.contains("window_width")) window_width = j["window_width"];
        if (j.contains("window_height")) window_height = j["window_height"];
        if (j.contains("render_fps")) render_fps = j["render_fps"];
        if (j.contains("show_trails")) show_trails = j["show_trails"];
        if (j.contains("trail_length")) trail_length = j["trail_length"];
        
        if (j.contains("enable_stats")) enable_stats = j["enable_stats"];
        if (j.contains("stats_interval")) stats_interval = j["stats_interval"];
        if (j.contains("stats_output_file")) stats_output_file = j["stats_output_file"];

        std::cout << "[Config] Loaded from " << filename << std::endl;
        return true;
    }
    catch (const json::parse_error& e) {
        std::cerr << "[Config] JSON parse error: " << e.what() << std::endl;
        return false;
    }
    catch (const std::exception& e) {
        std::cerr << "[Config] Error loading config: " << e.what() << std::endl;
        return false;
    }
}

bool SimulationConfig::save_to_file(const std::string& filename) const {
    try {
        json j;
        
        j["num_agents"] = num_agents;
        j["dt"] = dt;
        j["max_steps"] = max_steps;
        j["seed"] = seed;
        j["boundary"] = boundary;
        j["predator_chance"] = predator_chance;
        j["initial_energy"] = initial_energy;
        j["max_energy"] = max_energy;
        j["energy_consumption_rate"] = energy_consumption_rate;
        j["energy_gain_from_prey"] = energy_gain_from_prey;
        j["reproduction_energy_threshold"] = reproduction_energy_threshold;
        j["reproduction_energy_cost"] = reproduction_energy_cost;
        j["enable_ai"] = enable_ai;
        j["neural_input_size"] = neural_input_size;
        j["neural_hidden_size"] = neural_hidden_size;
        j["neural_output_size"] = neural_output_size;
        j["mutation_rate"] = mutation_rate;
        j["mutation_strength"] = mutation_strength;
        j["predator_chase_strength"] = predator_chase_strength;
        j["prey_flee_strength"] = prey_flee_strength;
        j["separation_strength"] = separation_strength;
        j["interaction_range"] = interaction_range;
        j["separation_range"] = separation_range;
        j["eating_range"] = eating_range;
        j["grid_cells"] = grid_cells;
        j["window_width"] = window_width;
        j["window_height"] = window_height;
        j["render_fps"] = render_fps;
        j["show_trails"] = show_trails;
        j["trail_length"] = trail_length;
        j["enable_stats"] = enable_stats;
        j["stats_interval"] = stats_interval;
        j["stats_output_file"] = stats_output_file;

        std::ofstream file(filename);
        if (!file.is_open()) {
            std::cerr << "[Config] Failed to create file: " << filename << std::endl;
            return false;
        }

        file << j.dump(2) << std::endl;  // Pretty print with 2-space indent
        std::cout << "[Config] Saved to " << filename << std::endl;
        return true;
    }
    catch (const std::exception& e) {
        std::cerr << "[Config] Error saving config: " << e.what() << std::endl;
        return false;
    }
}

SimulationConfig SimulationConfig::create_default() {
    return SimulationConfig{};
}
