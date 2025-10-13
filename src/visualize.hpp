#pragma once
#include "world.hpp"

class Visualizer {
public:
    bool init(int width, int height);
    void draw(const World& world);
    void shutdown();
};
