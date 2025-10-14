#include "scheduler.hpp"
#include "world.hpp"
#include "visualize.hpp"
#include <SDL2/SDL.h>
#include <iostream>
#include <thread>

int main() {
    constexpr double dt = 0.1;
    constexpr int steps = 2000;

    World world(10, 42);
    Visualizer viz;
    viz.init(800, 800);

    Scheduler scheduler(dt, steps, /*seed=*/42);
    scheduler.enable_profiling(true);

    bool running = true;
    bool paused = false;
    SDL_Event e;

    scheduler.set_on_start([]() {
        std::cout << "[Polaris] Simulation started.\n";
    });

    scheduler.set_on_end([]() {
        std::cout << "[Polaris] Simulation ended.\n";
    });

    scheduler.set_on_step([&](int step, double dt) {
        if (step % 100 == 0)
            std::cout << "[Tick " << step << "]" << std::endl;
    });

    scheduler.run([&](double stepDt, int step) {
        // Handle events each frame
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) running = false;
            if (e.type == SDL_KEYDOWN) {
                switch (e.key.keysym.sym) {
                    case SDLK_ESCAPE: running = false; break;
                    case SDLK_SPACE:
                        paused = !paused;
                        std::cout << (paused ? "[Paused]\n" : "[Resumed]\n");
                        break;
                }
            }
        }

        if (!running) return;
        if (paused) {
            SDL_Delay(16);
            return;
        }

        world.update(stepDt);
        viz.draw(world);

        // Slow down visually if needed
        SDL_Delay(16); // ~60 FPS
    });

    viz.shutdown();
    std::cout << "[Polaris] Clean shutdown.\n";
    return 0;
}
