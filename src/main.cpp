#include <iostream>
#include <vector>
#include <random>
#include <string>
#include <thread>
#include <chrono>
#include <cmath>

struct Vec2 {
    double x, y;
};

struct Agent {
    Vec2 pos, vel;
    bool predator; // true = predator, false = prey
};

int main() {
    constexpr double dt = 0.1;
    constexpr int steps = 300;
    constexpr int gridSize = 25;
    constexpr double boundary = 6.0;
    constexpr int frameDelayMs = 80; // ~12 FPS

    // Trail intensity map
    std::vector<std::vector<double>> trail(gridSize, std::vector<double>(gridSize, 0.0));

    std::mt19937 rng(42); // deterministic seed
    std::uniform_real_distribution<double> dist(-1.0, 1.0);
    std::bernoulli_distribution predatorChance(0.3); // 30% predators

    // initialize agents
    std::vector<Agent> agents(10);
    for (auto& a : agents) {
        a.pos = { dist(rng) * boundary, dist(rng) * boundary };
        a.vel = { dist(rng), dist(rng) };
        a.predator = predatorChance(rng);
    }

    for (int t = 0; t < steps; ++t) {
        // --- interactions ---
        for (auto& a : agents) {
            for (auto& b : agents) {
                if (&a == &b) continue;
                double dx = b.pos.x - a.pos.x;
                double dy = b.pos.y - a.pos.y;
                double dist2 = dx * dx + dy * dy + 1e-6;

                // predator chases, prey avoids
                if (a.predator && !b.predator && dist2 < 16.0) {
                    a.vel.x += 0.02 * dx;
                    a.vel.y += 0.02 * dy;
                } else if (!a.predator && b.predator && dist2 < 16.0) {
                    a.vel.x -= 0.03 * dx;
                    a.vel.y -= 0.03 * dy;
                }

                // general soft repulsion for everyone
                if (dist2 < 4.0) {
                    a.vel.x -= 0.02 * dx;
                    a.vel.y -= 0.02 * dy;
                }
            }
        }

        // --- update positions ---
        for (auto& a : agents) {
            a.pos.x += a.vel.x * dt;
            a.pos.y += a.vel.y * dt;

            // bounce off walls
            if (a.pos.x > boundary || a.pos.x < -boundary) a.vel.x *= -1;
            if (a.pos.y > boundary || a.pos.y < -boundary) a.vel.y *= -1;
        }

        // --- render ---
        // fade trails
        for (auto& row : trail)
            for (auto& val : row)
                val *= 0.9; // fade factor

        std::vector<std::string> grid(gridSize, std::string(gridSize, ' '));

        for (auto& a : agents) {
            int gx = static_cast<int>((a.pos.x / boundary) * (gridSize / 2) + gridSize / 2);
            int gy = static_cast<int>((a.pos.y / boundary) * (gridSize / 2) + gridSize / 2);
            if (gx >= 0 && gx < gridSize && gy >= 0 && gy < gridSize) {
                grid[gy][gx] = a.predator ? 'P' : 'p';
                trail[gy][gx] = 1.0; // refresh trail intensity
            }
        }

        // clear screen
        std::cout << "\033[2J\033[H";
        std::cout << "Step " << t << "  (Predators = red, Prey = cyan)\n";

        for (int y = 0; y < gridSize; ++y) {
            for (int x = 0; x < gridSize; ++x) {
                double intensity = trail[y][x];
                if (grid[y][x] == 'P') {
                    std::cout << "\033[31m" << 'P' << "\033[0m"; // red predator
                } else if (grid[y][x] == 'p') {
                    std::cout << "\033[36m" << 'p' << "\033[0m"; // cyan prey
                } else if (intensity > 0.1) {
                    // gray fading trail (brightness based on intensity)
                    int shade = static_cast<int>(232 + std::clamp(intensity * 20.0, 0.0, 23.0));
                    std::cout << "\033[48;5;" << shade << "m \033[0m";
                } else {
                    std::cout << '.';
                }
            }
            std::cout << "\n";
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(frameDelayMs));
    }

    std::cout << "\nSimulation finished.\n";
    return 0;
}
