#pragma once
#include "graphics/Framebuffer.hpp"
#include "world/Sector.hpp"
#include "gameplay/Player.hpp"

struct Renderer
{
    float fov{1.04719755f}; // 60 degrees in radians
    float nearZ{0.05f};     // Near clipping plane

    void render(Framebuffer& fb, const Sector& sector, const Player& player);

private:
    void drawWallSegment(Framebuffer& fb,
                         float x0, float z0, float x1, float z1,
                         float floorH, float ceilH,
                         float playerEyeZ,
                         uint32_t wallColor);
};
