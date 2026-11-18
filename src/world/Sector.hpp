#pragma once
#include "world/Wall.hpp"
#include <vector>
#include <cstdint>

struct Sector
{
    float floorHeight{0.0f};
    float ceilingHeight{3.0f};

    uint32_t floorColor{0xFF2A2A2A};
    uint32_t ceilingColor{0xFF1A1A1A};

    std::vector<Wall> walls;

    Sector() = default;
    Sector(float floorH, float ceilH, uint32_t floorCol, uint32_t ceilCol)
        : floorHeight(floorH), ceilingHeight(ceilH), floorColor(floorCol), ceilingColor(ceilCol)
    {
    }

    void addWall(const Vec2& p0, const Vec2& p1, int portal = -1, uint32_t color = 0xFF808080)
    {
        walls.emplace_back(p0, p1, portal, color);
    }
};
