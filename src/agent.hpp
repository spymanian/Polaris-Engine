#pragma once

struct Vec2 { double x, y; };

struct Agent {
    Vec2 pos, vel;
    bool predator = false;
};
