#include "spatial_grid.hpp"
#include <cmath>
#include <algorithm>

SpatialGrid::SpatialGrid(double world_size, int grid_cells)
    : world_size_(world_size), grid_cells_(grid_cells) {
    cell_size_ = (2.0 * world_size_) / grid_cells_;
    cells_.resize(grid_cells_ * grid_cells_);
}

void SpatialGrid::clear() {
    for (auto& cell : cells_) {
        cell.clear();
    }
}

int SpatialGrid::to_grid_x(double x) const {
    return static_cast<int>((x + world_size_) / cell_size_);
}

int SpatialGrid::to_grid_y(double y) const {
    return static_cast<int>((y + world_size_) / cell_size_);
}

int SpatialGrid::to_cell_index(int gx, int gy) const {
    return gy * grid_cells_ + gx;
}

bool SpatialGrid::in_bounds(int gx, int gy) const {
    return gx >= 0 && gx < grid_cells_ && gy >= 0 && gy < grid_cells_;
}

void SpatialGrid::insert(size_t agent_idx, const Vec2& pos) {
    int gx = to_grid_x(pos.x);
    int gy = to_grid_y(pos.y);
    
    if (in_bounds(gx, gy)) {
        cells_[to_cell_index(gx, gy)].push_back(agent_idx);
    }
}

void SpatialGrid::query_radius(const Vec2& pos, double radius, 
                               std::function<void(size_t)> callback) const {
    int center_gx = to_grid_x(pos.x);
    int center_gy = to_grid_y(pos.y);
    
    // Check how many cells we need to search
    int cell_radius = static_cast<int>(std::ceil(radius / cell_size_)) + 1;
    
    for (int dy = -cell_radius; dy <= cell_radius; ++dy) {
        for (int dx = -cell_radius; dx <= cell_radius; ++dx) {
            int gx = center_gx + dx;
            int gy = center_gy + dy;
            
            if (!in_bounds(gx, gy)) continue;
            
            const auto& cell = cells_[to_cell_index(gx, gy)];
            for (size_t idx : cell) {
                callback(idx);
            }
        }
    }
}

void SpatialGrid::query_cell(const Vec2& pos, std::function<void(size_t)> callback) const {
    int gx = to_grid_x(pos.x);
    int gy = to_grid_y(pos.y);
    
    if (!in_bounds(gx, gy)) return;
    
    const auto& cell = cells_[to_cell_index(gx, gy)];
    for (size_t idx : cell) {
        callback(idx);
    }
}
