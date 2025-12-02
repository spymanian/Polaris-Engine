#pragma once
#include "config.hpp"
#include <SDL2/SDL.h>

// Forward declarations
struct ImGuiContext;

class ImGuiPanel {
public:
    ImGuiPanel();
    ~ImGuiPanel();

    bool init(SDL_Window* window, SDL_Renderer* renderer);
    void shutdown();
    
    bool process_event(SDL_Event* event);
    void begin_frame();
    void render(SimulationConfig& config, const class Statistics* stats, class World* world = nullptr);
    void end_frame();

    bool wants_capture_mouse() const;
    bool wants_capture_keyboard() const;

private:
    bool initialized_ = false;
    bool show_config_window_ = true;
    bool show_stats_window_ = true;
    SDL_Window* window_ = nullptr;
    SDL_Renderer* renderer_ = nullptr;

    void render_config_panel(SimulationConfig& config, class World* world);
    void render_stats_panel(const Statistics* stats);
};
