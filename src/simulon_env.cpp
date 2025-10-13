#include "simulon_env.hpp"
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <SDL2/SDL.h>
#include <vector>        // <-- needed for std::vector
#include <string>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

namespace py = pybind11;

SimulonEnv::SimulonEnv(int nAgents, unsigned seed, double dt)
    : world_(nAgents, seed), dt_(dt) {}

void SimulonEnv::step() {
    world_.update(dt_);
}

std::vector<Agent> SimulonEnv::get_state() const {
    return world_.agents;
}

// ✅ FIXED VERSION of render_frame
void SimulonEnv::render_frame(const std::string& filename, int size) const {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        throw std::runtime_error("SDL initialization failed");
    }

    SDL_Window* win = SDL_CreateWindow("offscreen",
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        size, size, SDL_WINDOW_HIDDEN);
    SDL_Renderer* renderer = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);

    // clear background
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    const double scale = size / (2.0 * world_.boundary);
    for (auto& a : world_.agents) {
        int px = static_cast<int>((a.pos.x + world_.boundary) * scale);
        int py = static_cast<int>((a.pos.y + world_.boundary) * scale);
        SDL_Rect r{px - 4, py - 4, 8, 8};

        if (a.predator) SDL_SetRenderDrawColor(renderer, 255, 50, 50, 255);
        else             SDL_SetRenderDrawColor(renderer, 50, 200, 255, 255);

        SDL_RenderFillRect(renderer, &r);
    }

    SDL_RenderPresent(renderer);

    // read pixels from renderer
    std::vector<unsigned char> pixels(size * size * 4);
    SDL_RenderReadPixels(renderer, nullptr, SDL_PIXELFORMAT_ABGR8888,
                         pixels.data(), size * 4);

    // write PNG
    if (!stbi_write_png(filename.c_str(), size, size, 4, pixels.data(), size * 4)) {
        throw std::runtime_error("Failed to write PNG file");
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(win);
    SDL_QuitSubSystem(SDL_INIT_VIDEO);
}

// 🔗 Python binding
PYBIND11_MODULE(simulon, m) {
    m.doc() = "Deterministic C++ simulation engine for AI agents";

    py::class_<Vec2>(m, "Vec2")
    .def_readonly("x", &Vec2::x)
    .def_readonly("y", &Vec2::y);

py::class_<Agent>(m, "Agent")
    .def_readonly("pos", &Agent::pos)
    .def_readonly("vel", &Agent::vel)
    .def_readonly("predator", &Agent::predator);

    py::class_<SimulonEnv>(m, "SimulonEnv")
        .def(py::init<int, unsigned, double>(),
             py::arg("n_agents")=10, py::arg("seed")=42, py::arg("dt")=0.1)
        .def("step", &SimulonEnv::step)
        .def("get_state", &SimulonEnv::get_state)
        .def("render_frame", &SimulonEnv::render_frame,
             py::arg("filename"), py::arg("size") = 600);
}
