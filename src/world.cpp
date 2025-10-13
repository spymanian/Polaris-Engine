#include "world.hpp"
#include <random>
#include <cmath>

World::World(int nAgents, unsigned seed) {
    std::mt19937 rng(seed);
    std::uniform_real_distribution<double> dist(-1.0, 1.0);
    std::bernoulli_distribution predatorChance(0.3);

    agents.resize(nAgents);
    for (auto& a : agents) {
        a.pos = {dist(rng) * boundary, dist(rng) * boundary};
        a.vel = {dist(rng), dist(rng)};
        a.predator = predatorChance(rng);
    }
}

void World::update(double dt) {
    // interactions
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
            if (dist2 < 4.0) {
                a.vel.x -= 0.02 * dx;
                a.vel.y -= 0.02 * dy;
            }
        }
    }

    // integrate + wall bounce
    for (auto& a : agents) {
        a.pos.x += a.vel.x * dt;
        a.pos.y += a.vel.y * dt;
        if (a.pos.x > boundary || a.pos.x < -boundary) a.vel.x *= -1;
        if (a.pos.y > boundary || a.pos.y < -boundary) a.vel.y *= -1;
    }
}
