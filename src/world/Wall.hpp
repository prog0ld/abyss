#pragma once
#include "math/Vec2.hpp"
#include <cstdint>

struct Wall
{
    Vec2 p0; // Start point in world space
    Vec2 p1; // End point in world space

    int portalSector{-1}; // -1 if solid wall, >= 0 if portal to adjacent sector
    uint32_t color{0xFF808080}; // Shading/material tint for flat-shading phase

    Wall() = default;
    Wall(const Vec2& inP0, const Vec2& inP1, int portal = -1, uint32_t col = 0xFF808080)
        : p0(inP0), p1(inP1), portalSector(portal), color(col)
    {
    }
};
