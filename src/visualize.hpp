#pragma once
#include "world.hpp"
#include <SDL2/SDL.h>

class Visualizer {
public:
    bool init(int width, int height);
    void draw(const World& world);
    void shutdown();
    
    SDL_Window* get_window();
    SDL_Renderer* get_renderer();
};
