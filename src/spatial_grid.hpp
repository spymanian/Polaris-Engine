#pragma once
#include <vector>
#include <functional>
#include "agent.hpp"

class SpatialGrid {
public:
    SpatialGrid(double world_size, int grid_cells);
    
    void clear();
    void insert(size_t agent_idx, const Vec2& pos);
    
    // Query agents within a radius of a position
    void query_radius(const Vec2& pos, double radius, 
                     std::function<void(size_t)> callback) const;
    
    // Get all agents in the same cell
    void query_cell(const Vec2& pos, std::function<void(size_t)> callback) const;

private:
    double world_size_;
    int grid_cells_;
    double cell_size_;
    std::vector<std::vector<size_t>> cells_;
    
    int to_grid_x(double x) const;
    int to_grid_y(double y) const;
    int to_cell_index(int gx, int gy) const;
    bool in_bounds(int gx, int gy) const;
};
