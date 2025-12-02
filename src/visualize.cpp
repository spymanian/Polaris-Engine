#include "visualize.hpp"
#include <SDL2/SDL.h>
#include <algorithm>

static SDL_Window* window = nullptr;
static SDL_Renderer* renderer = nullptr;

bool Visualizer::init(int width, int height) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) return false;
    window = SDL_CreateWindow("Polaris", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                              width, height, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    return true;
}

void Visualizer::draw(const World& world) {
    // Get current window size
    int window_width, window_height;
    SDL_GetWindowSize(window, &window_width, &window_height);
    
    // Use the smaller dimension to keep square aspect ratio
    int viewport_size = std::min(window_width, window_height);
    const double scale = viewport_size / (2.0 * world.boundary);
    
    // Clear with black
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    
    // Draw fade effect over entire viewport
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 40);
    SDL_Rect fade{0, 0, viewport_size, viewport_size};
    SDL_RenderFillRect(renderer, &fade);

    for (auto& a : world.agents) {
        if (!a.alive) continue;

        // Draw trail first (so agents render on top)
        if (!a.trail.empty()) {
            for (size_t i = 1; i < a.trail.size(); ++i) {
                int px1 = static_cast<int>((a.trail[i-1].x + world.boundary) * scale);
                int py1 = static_cast<int>((a.trail[i-1].y + world.boundary) * scale);
                int px2 = static_cast<int>((a.trail[i].x + world.boundary) * scale);
                int py2 = static_cast<int>((a.trail[i].y + world.boundary) * scale);
                
                // Fade trail from dark to bright
                int alpha = 30 + (i * 225 / a.trail.size());
                
                if (a.predator) {
                    SDL_SetRenderDrawColor(renderer, 255, 50, 50, alpha);
                } else {
                    SDL_SetRenderDrawColor(renderer, 50, 200, 255, alpha);
                }
                
                SDL_RenderDrawLine(renderer, px1, py1, px2, py2);
            }
        }

        // Draw agent
        int px = static_cast<int>((a.pos.x + world.boundary) * scale);
        int py = static_cast<int>((a.pos.y + world.boundary) * scale);
        
        // Size based on energy (4-10 pixels)
        int size = 4 + static_cast<int>(std::clamp(a.energy / 40.0, 0.0, 6.0));
        SDL_Rect r{px - size/2, py - size/2, size, size};
        
        if (a.predator) {
            // Red for predators, brightness based on energy
            int brightness = 50 + static_cast<int>(std::clamp(a.energy * 2.0, 0.0, 205.0));
            SDL_SetRenderDrawColor(renderer, brightness, 50, 50, 255);
        } else {
            // Blue for prey, brightness based on energy
            int brightness = 50 + static_cast<int>(std::clamp(a.energy * 1.5, 0.0, 205.0));
            SDL_SetRenderDrawColor(renderer, 50, brightness, 255, 255);
        }
        SDL_RenderFillRect(renderer, &r);
    }
    // Don't present here - let ImGui render on top first
}

void Visualizer::shutdown() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

SDL_Window* Visualizer::get_window() {
    return window;
}

SDL_Renderer* Visualizer::get_renderer() {
    return renderer;
}
