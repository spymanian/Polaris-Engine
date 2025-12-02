#include "imgui_panel.hpp"
#include "statistics.hpp"
#include "world.hpp"
#include "../external/imgui.h"
#include "../external/imgui_impl_sdl2.h"
#include "../external/imgui_impl_sdlrenderer2.h"
#include <iostream>

ImGuiPanel::ImGuiPanel() {}

ImGuiPanel::~ImGuiPanel() {
    shutdown();
}

bool ImGuiPanel::init(SDL_Window* window, SDL_Renderer* renderer) {
    window_ = window;
    renderer_ = renderer;

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();

    // Setup Platform/Renderer backends
    if (!ImGui_ImplSDL2_InitForSDLRenderer(window, renderer)) {
        std::cerr << "[ImGui] Failed to initialize SDL2 backend\n";
        return false;
    }
    
    if (!ImGui_ImplSDLRenderer2_Init(renderer)) {
        std::cerr << "[ImGui] Failed to initialize SDL Renderer backend\n";
        return false;
    }

    initialized_ = true;
    std::cout << "[ImGui] Initialized successfully\n";
    std::cout << "[ImGui] Press 'G' to toggle UI, 'C' for config, 'S' for stats\n";
    return true;
}

void ImGuiPanel::shutdown() {
    if (initialized_) {
        ImGui_ImplSDLRenderer2_Shutdown();
        ImGui_ImplSDL2_Shutdown();
        ImGui::DestroyContext();
        initialized_ = false;
    }
}

bool ImGuiPanel::process_event(SDL_Event* event) {
    if (!initialized_) return false;
    
    // Toggle windows with keyboard
    if (event->type == SDL_KEYDOWN) {
        switch (event->key.keysym.sym) {
            case SDLK_g:
                show_config_window_ = !show_config_window_;
                show_stats_window_ = !show_stats_window_;
                return true;
            case SDLK_c:
                show_config_window_ = !show_config_window_;
                return true;
            case SDLK_s:
                show_stats_window_ = !show_stats_window_;
                return true;
        }
    }
    
    return ImGui_ImplSDL2_ProcessEvent(event);
}

bool ImGuiPanel::wants_capture_mouse() const {
    if (!initialized_) return false;
    return ImGui::GetIO().WantCaptureMouse;
}

bool ImGuiPanel::wants_capture_keyboard() const {
    if (!initialized_) return false;
    return ImGui::GetIO().WantCaptureKeyboard;
}

void ImGuiPanel::begin_frame() {
    if (!initialized_) return;
    ImGui_ImplSDLRenderer2_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();
}

void ImGuiPanel::render(SimulationConfig& config, const Statistics* stats, World* world) {
    if (!initialized_) return;

    if (show_config_window_) {
        render_config_panel(config, world);
    }

    if (show_stats_window_ && stats) {
        render_stats_panel(stats);
    }
}

void ImGuiPanel::render_config_panel(SimulationConfig& config, World* world) {
    ImGui::Begin("Simulation Config", &show_config_window_);
    
    ImGui::Text("Controls: [SPACE] Pause | [ESC] Quit");
    ImGui::Text("[G] Toggle UI | [C] Config | [S] Stats | [T] Trails");
    ImGui::Separator();
    
    // AI Toggle
    ImGui::Text("AI System");
    if (ImGui::Checkbox("Enable AI Control", &config.enable_ai)) {
        std::cout << (config.enable_ai ? "[AI] Enabled - Agents now use neural networks\n" : "[AI] Disabled - Using scripted behavior\n");
    }
    
    if (config.enable_ai) {
        ImGui::Indent();
        float mut_rate = static_cast<float>(config.mutation_rate);
        if (ImGui::SliderFloat("Mutation Rate", &mut_rate, 0.0f, 0.5f)) {
            config.mutation_rate = mut_rate;
        }
        float mut_str = static_cast<float>(config.mutation_strength);
        if (ImGui::SliderFloat("Mutation Strength", &mut_str, 0.0f, 1.0f)) {
            config.mutation_strength = mut_str;
        }
        ImGui::Unindent();
    }
    
    ImGui::Separator();
    
    // Agent parameters
    ImGui::Text("Population Controls");
    if (ImGui::Button("Spawn 10 Prey")) {
        if (world) {
            world->spawn_prey(10);
            std::cout << "[UI] Spawned 10 prey\n";
        }
    }
    ImGui::SameLine();
    if (ImGui::Button("Spawn 5 Predators")) {
        if (world) {
            world->spawn_predators(5);
            std::cout << "[UI] Spawned 5 predators\n";
        }
    }
    
    ImGui::Separator();
    float predator_chance = static_cast<float>(config.predator_chance);
    if (ImGui::SliderFloat("Predator Spawn %", &predator_chance, 0.0f, 1.0f)) {
        config.predator_chance = predator_chance;
    }
    
    // Energy parameters
    ImGui::Separator();
    ImGui::Text("Energy System (Rebalanced)");
    float consumption = static_cast<float>(config.energy_consumption_rate);
    if (ImGui::SliderFloat("Consumption Rate", &consumption, 0.0f, 2.0f)) {
        config.energy_consumption_rate = consumption;
    }
    
    float gain = static_cast<float>(config.energy_gain_from_prey);
    if (ImGui::SliderFloat("Energy from Prey", &gain, 0.0f, 150.0f)) {
        config.energy_gain_from_prey = gain;
    }
    
    float repro_threshold = static_cast<float>(config.reproduction_energy_threshold);
    if (ImGui::SliderFloat("Reproduction Threshold", &repro_threshold, 50.0f, 200.0f)) {
        config.reproduction_energy_threshold = repro_threshold;
    }
    
    // Interaction parameters
    ImGui::Separator();
    ImGui::Text("Interaction Forces");
    if (!config.enable_ai) {
        float chase = static_cast<float>(config.predator_chase_strength);
        if (ImGui::SliderFloat("Chase Strength", &chase, 0.0f, 0.1f)) {
            config.predator_chase_strength = chase;
        }
        
        float flee = static_cast<float>(config.prey_flee_strength);
        if (ImGui::SliderFloat("Flee Strength", &flee, 0.0f, 0.1f)) {
            config.prey_flee_strength = flee;
        }
    } else {
        ImGui::TextDisabled("Chase/Flee (AI Controlled)");
    }
    
    float separation = static_cast<float>(config.separation_strength);
    if (ImGui::SliderFloat("Separation", &separation, 0.0f, 0.1f)) {
        config.separation_strength = separation;
    }
    
    // Visualization
    ImGui::Separator();
    ImGui::Text("Visualization");
    if (ImGui::Checkbox("Show Trails", &config.show_trails)) {
        std::cout << (config.show_trails ? "[Trails ON]\n" : "[Trails OFF]\n");
    }
    if (config.show_trails) {
        int trail_len = config.trail_length;
        if (ImGui::SliderInt("Trail Length", &trail_len, 5, 50)) {
            config.trail_length = trail_len;
        }
    }
    
    ImGui::Separator();
    if (ImGui::Button("Save Config")) {
        config.save_to_file("config.json");
        std::cout << "[Config] Saved to config.json\n";
    }
    ImGui::SameLine();
    if (ImGui::Button("Reload Config")) {
        config.load_from_file("config.json");
        std::cout << "[Config] Reloaded from config.json\n";
    }
    
    ImGui::End();
}

void ImGuiPanel::render_stats_panel(const Statistics* stats) {
    ImGui::Begin("Statistics & AI Evolution", &show_stats_window_);
    
    ImGui::Text("Total Births: %d", stats->total_births());
    ImGui::Text("Total Deaths: %d", stats->total_deaths());
    
    const auto& history = stats->get_history();
    if (!history.empty()) {
        const auto& latest = history.back();
        ImGui::Separator();
        ImGui::Text("Current Population: %d", latest.total_agents);
        ImGui::Text("  Predators: %d", latest.predators);
        ImGui::Text("  Prey: %d", latest.prey);
        
        // Balance indicator
        if (latest.predators > 0 && latest.prey > 0) {
            float ratio = static_cast<float>(latest.prey) / static_cast<float>(latest.predators);
            ImGui::Text("  Prey:Predator Ratio: %.2f:1", ratio);
            if (ratio < 2.0f) {
                ImGui::TextColored(ImVec4(1.0f, 0.3f, 0.3f, 1.0f), "  WARNING: Low prey!");
            } else if (ratio > 8.0f) {
                ImGui::TextColored(ImVec4(1.0f, 0.7f, 0.3f, 1.0f), "  Too many prey");
            } else {
                ImGui::TextColored(ImVec4(0.3f, 1.0f, 0.3f, 1.0f), "  Balanced ecosystem");
            }
        }
        
        ImGui::Separator();
        ImGui::Text("Average Energy: %.1f", latest.avg_energy);
        ImGui::Text("  Predator Energy: %.1f", latest.avg_predator_energy);
        ImGui::Text("  Prey Energy: %.1f", latest.avg_prey_energy);
        
        // Simple population graph
        if (history.size() > 1) {
            ImGui::Separator();
            ImGui::Text("Population History:");
            
            std::vector<float> pop_data;
            std::vector<float> pred_data;
            std::vector<float> prey_data;
            
            size_t start = history.size() > 100 ? history.size() - 100 : 0;
            for (size_t i = start; i < history.size(); ++i) {
                pop_data.push_back(static_cast<float>(history[i].total_agents));
                pred_data.push_back(static_cast<float>(history[i].predators));
                prey_data.push_back(static_cast<float>(history[i].prey));
            }
            
            ImGui::PlotLines("Total", pop_data.data(), pop_data.size(), 0, nullptr, 0.0f, 
                           *std::max_element(pop_data.begin(), pop_data.end()) * 1.1f, ImVec2(0, 80));
            ImGui::PlotLines("Predators", pred_data.data(), pred_data.size(), 0, nullptr, 0.0f, 
                           *std::max_element(pred_data.begin(), pred_data.end()) * 1.1f, ImVec2(0, 80));
            ImGui::PlotLines("Prey", prey_data.data(), prey_data.size(), 0, nullptr, 0.0f, 
                           *std::max_element(prey_data.begin(), prey_data.end()) * 1.1f, ImVec2(0, 80));
        }
    }
    
    ImGui::End();
}

void ImGuiPanel::end_frame() {
    if (!initialized_) return;
    ImGui::Render();
    ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData(), renderer_);
    SDL_RenderPresent(renderer_);  // Present after ImGui renders
}
