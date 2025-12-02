cmake_minimum_required(VERSION 3.14)
project(Polaris LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# --- Dependencies ---
find_package(SDL2 REQUIRED)
find_package(pybind11 REQUIRED)

# --- Main Executable ---
add_executable(polaris
    src/main.cpp
    src/scheduler.cpp
    src/world.cpp
    src/agent.cpp
    src/visualize.cpp
    src/config.cpp
    src/statistics.cpp
    src/spatial_grid.cpp
    src/imgui_panel.cpp
    src/neural_network.cpp
    # ImGui core files
    external/imgui.cpp
    external/imgui_widgets.cpp
    external/imgui_tables.cpp
    external/imgui_draw.cpp
    # ImGui SDL2 backend
    external/imgui_impl_sdl2.cpp
    external/imgui_impl_sdlrenderer2.cpp
)
target_include_directories(polaris PRIVATE ${SDL2_INCLUDE_DIRS} src external)
target_link_libraries(polaris PRIVATE ${SDL2_LIBRARIES})

# --- Python Module ---
pybind11_add_module(simulon
    src/simulon_env.cpp
    src/world.cpp
    src/agent.cpp
    src/scheduler.cpp
    src/config.cpp
    src/statistics.cpp
    src/spatial_grid.cpp
    src/neural_network.cpp
)
target_include_directories(simulon PRIVATE src external)
