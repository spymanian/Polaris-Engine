#include <SDL2/SDL.h>
#include <vector>
#include <random>
#include <chrono>
#include <cmath>

struct Vec2 { double x, y; };
struct Agent { Vec2 pos, vel; bool predator; };

int main() {
    constexpr int winSize = 600;
    constexpr int gridSize = 25;
    constexpr double boundary = 6.0;
    constexpr double dt = 0.1;
    constexpr int steps = 2000;
    constexpr double scale = winSize / (2.0 * boundary);

    if (SDL_Init(SDL_INIT_VIDEO) < 0) return -1;
    SDL_Window* window = SDL_CreateWindow(
        "Polaris Simulation", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        winSize, winSize, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    // deterministic RNG
    std::mt19937 rng(42);
    std::uniform_real_distribution<double> dist(-1.0, 1.0);
    std::bernoulli_distribution predatorChance(0.3);

    std::vector<Agent> agents(10);
    for (auto& a : agents) {
        a.pos = { dist(rng) * boundary, dist(rng) * boundary };
        a.vel = { dist(rng), dist(rng) };
        a.predator = predatorChance(rng);
    }

    bool running = true;
    SDL_Event e;
    auto start = std::chrono::steady_clock::now();

    // main loop
    for (int t = 0; t < steps && running; ++t) {
        while (SDL_PollEvent(&e))
            if (e.type == SDL_QUIT) running = false;

        // --- interactions ---
        for (auto& a : agents) {
            for (auto& b : agents) {
                if (&a == &b) continue;
                double dx = b.pos.x - a.pos.x;
                double dy = b.pos.y - a.pos.y;
                double dist2 = dx*dx + dy*dy + 1e-6;

                if (a.predator && !b.predator && dist2 < 16.0) {
                    a.vel.x += 0.02 * dx;
                    a.vel.y += 0.02 * dy;
                } else if (!a.predator && b.predator && dist2 < 16.0) {
                    a.vel.x -= 0.03 * dx;
                    a.vel.y -= 0.03 * dy;
                }
                if (dist2 < 4.0) { // mild repulsion
                    a.vel.x -= 0.02 * dx;
                    a.vel.y -= 0.02 * dy;
                }
            }
        }

        // --- update positions ---
        for (auto& a : agents) {
            a.pos.x += a.vel.x * dt;
            a.pos.y += a.vel.y * dt;
            if (a.pos.x > boundary || a.pos.x < -boundary) a.vel.x *= -1;
            if (a.pos.y > boundary || a.pos.y < -boundary) a.vel.y *= -1;
        }

        // --- draw ---
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        for (auto& a : agents) {
            int px = static_cast<int>((a.pos.x + boundary) * scale);
            int py = static_cast<int>((a.pos.y + boundary) * scale);
            SDL_Rect r{px - 4, py - 4, 8, 8};

            if (a.predator) SDL_SetRenderDrawColor(renderer, 255, 50, 50, 255);
            else            SDL_SetRenderDrawColor(renderer, 50, 200, 255, 255);

            SDL_RenderFillRect(renderer, &r);
        }

        SDL_RenderPresent(renderer);
        SDL_Delay(16); // ~60 FPS
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
