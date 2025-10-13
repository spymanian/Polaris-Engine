#pragma once
#include <vector>
#include "agent.hpp"

struct World {
    std::vector<Agent> agents;
    double boundary = 6.0;

    World(int nAgents, unsigned seed);
    void update(double dt);
};
