#include "scheduler.hpp"
#include "world.hpp"
#include "visualize.hpp"
#include <SDL2/SDL.h>

int main() {
    const double dt = 0.1;
    const int steps = 2000;

    World world(10, 42);
    Visualizer viz;
    viz.init(600, 600);

    Scheduler scheduler(dt, steps);
    bool running = true;
    SDL_Event e;

    scheduler.run([&](double stepDt, int step) {
        while (SDL_PollEvent(&e))
            if (e.type == SDL_QUIT) running = false;

        if (!running) return;

        world.update(stepDt);
        viz.draw(world);
        SDL_Delay(16); // ~60 FPS
    });

    viz.shutdown();
    return 0;
}
