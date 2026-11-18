#pragma once
#include "math/Vec2.hpp"
#include <cstdint>

struct Wall
{
    Vec2 p0;
    Vec2 p1;
    uint32_t color{0xFF808080};

    Wall() = default;
    Wall(const Vec2& a, const Vec2& b, uint32_t col = 0xFF808080)
        : p0(a), p1(b), color(col) {}
};
