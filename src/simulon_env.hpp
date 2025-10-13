#pragma once
#include "world.hpp"
#include <string>

class SimulonEnv {
    World world_;
    double dt_;
public:
    SimulonEnv(int nAgents = 10, unsigned seed = 42, double dt = 0.1);
    void step();
    std::vector<Agent> get_state() const;
    void render_frame(const std::string& filename, int size = 600) const;  // NEW
};
