#include "visualize.hpp"
#include <SDL2/SDL.h>

static SDL_Window* window = nullptr;
static SDL_Renderer* renderer = nullptr;

bool Visualizer::init(int width, int height) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) return false;
    window = SDL_CreateWindow("Polaris", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                              width, height, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    return true;
}

void Visualizer::draw(const World& world) {
    const double scale = 600 / (2.0 * world.boundary);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 40);
    SDL_Rect fade{0, 0, 600, 600};
    SDL_RenderFillRect(renderer, &fade);

    for (auto& a : world.agents) {
        int px = static_cast<int>((a.pos.x + world.boundary) * scale);
        int py = static_cast<int>((a.pos.y + world.boundary) * scale);
        SDL_Rect r{px - 4, py - 4, 8, 8};
        if (a.predator)
            SDL_SetRenderDrawColor(renderer, 255, 50, 50, 255);
        else
            SDL_SetRenderDrawColor(renderer, 50, 200, 255, 255);
        SDL_RenderFillRect(renderer, &r);
    }
    SDL_RenderPresent(renderer);
}

void Visualizer::shutdown() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
