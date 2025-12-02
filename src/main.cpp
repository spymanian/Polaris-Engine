#include "scheduler.hpp"
#include "world.hpp"
#include "visualize.hpp"
#include "config.hpp"
#include "statistics.hpp"
#include "imgui_panel.hpp"
#include <SDL2/SDL.h>
#include <iostream>
#include <thread>

int main() {
    // Load configuration
    SimulationConfig config = SimulationConfig::create_default();
    if (!config.load_from_file("config.json")) {
        std::cout << "[Main] Using default configuration\n";
        config.save_to_file("config.json"); // Save default for reference
    }

    World world(config, config.seed);
    Visualizer viz;
    viz.init(config.window_width, config.window_height);

    // Initialize statistics
    Statistics stats(config.stats_output_file, config.enable_stats);
    world.set_statistics(&stats);

    // Initialize ImGui
    ImGuiPanel gui;
    gui.init(viz.get_window(), viz.get_renderer());

    Scheduler scheduler(config.dt, config.max_steps, config.seed);
    scheduler.enable_profiling(false);

    bool running = true;
    bool paused = false;
    SDL_Event e;
    int current_step = 0;

    scheduler.set_on_start([&]() {
        std::cout << "[Polaris] Simulation started.\n";
        std::cout << "[Polaris] Initial population: " << world.agents.size()
                  << " (" << world.count_predators() << " predators, "
                  << world.count_prey() << " prey)\n";
    });

    scheduler.set_on_end([&]() {
        std::cout << "[Polaris] Simulation ended.\n";
        std::cout << "[Polaris] Final population: " << world.agents.size()
                  << " (" << world.count_predators() << " predators, "
                  << world.count_prey() << " prey)\n";
        std::cout << "[Polaris] Total births: " << stats.total_births() << "\n";
        std::cout << "[Polaris] Total deaths: " << stats.total_deaths() << "\n";
    });

    scheduler.set_on_step([&](int step, double dt) {
        current_step = step;
        if (step % config.stats_interval == 0) {
            std::cout << "[Tick " << step << "] Population: " << world.agents.size()
                     << " (P:" << world.count_predators() 
                     << " Y:" << world.count_prey() << ")" << std::endl;
            stats.record_step(step, step * dt, world);
        }
    });

    scheduler.run([&](double stepDt, int step) {
        // ImGui new frame
        gui.begin_frame();

        // Handle events each frame
        while (SDL_PollEvent(&e)) {
            gui.process_event(&e);

            if (e.type == SDL_QUIT) running = false;
            if (e.type == SDL_KEYDOWN) {
                switch (e.key.keysym.sym) {
                    case SDLK_ESCAPE: running = false; break;
                    case SDLK_SPACE:
                        paused = !paused;
                        std::cout << (paused ? "[Paused]\n" : "[Resumed]\n");
                        break;
                    case SDLK_r:
                        config.load_from_file("config.json");
                        std::cout << "[Config reloaded]\n";
                        break;
                    case SDLK_t:
                        config.show_trails = !config.show_trails;
                        std::cout << (config.show_trails ? "[Trails ON]\n" : "[Trails OFF]\n");
                        break;
                }
            }
        }

        if (!running) return;
        if (paused) {
            // Still render UI when paused
            viz.draw(world);
            gui.render(config, &stats, &world);
            gui.end_frame();
            SDL_Delay(16);
            return;
        }

        world.update(stepDt);
        viz.draw(world);

        // Render ImGui (after world draw so it appears on top)
        gui.render(config, &stats, &world);
        gui.end_frame();

        // Cap frame rate
        SDL_Delay(1000 / config.render_fps);
    });

    gui.shutdown();
    viz.shutdown();
    stats.flush();
    std::cout << "[Polaris] Clean shutdown.\n";
    return 0;
}
