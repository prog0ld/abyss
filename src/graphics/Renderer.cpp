#include "graphics/Renderer.hpp"
#include <cmath>
#include <algorithm>
#include <vector>

void Renderer::render(Framebuffer& fb, const Sector& sector, const Player& player)
{
    // 1. Clear background / render simple floor and ceiling planes
    int halfH = fb.height / 2;
    // Ceiling: rows 0 .. halfH
    for (int y = 0; y < halfH; ++y)
    {
        for (int x = 0; x < fb.width; ++x)
        {
            fb.pixels[y * fb.width + x] = sector.ceilingColor;
        }
    }
    // Floor: rows halfH .. height
    for (int y = halfH; y < fb.height; ++y)
    {
        for (int x = 0; x < fb.width; ++x)
        {
            fb.pixels[y * fb.width + x] = sector.floorColor;
        }
    }

    // Camera basis vectors
    float cosA = std::cos(player.angle);
    float sinA = std::sin(player.angle);

    // 2. Transform and render each wall
    for (const auto& wall : sector.walls)
    {
        // Vector from player to wall vertices
        float dx0 = wall.p0.x - player.position.x;
        float dy0 = wall.p0.y - player.position.y;
        float dx1 = wall.p1.x - player.position.x;
        float dy1 = wall.p1.y - player.position.y;

        // Transform to camera space
        // Forward is +Z, Right is +X
        float z0 = dx0 * cosA + dy0 * sinA;
        float x0 = -dx0 * sinA + dy0 * cosA;

        float z1 = dx1 * cosA + dy1 * sinA;
        float x1 = -dx1 * sinA + dy1 * cosA;

        // Near-plane clipping: if both points are behind camera, skip
        if (z0 <= nearZ && z1 <= nearZ)
        {
            continue;
        }

        // Clip point 0 against near plane
        if (z0 < nearZ)
        {
            float t = (nearZ - z0) / (z1 - z0);
            z0 = nearZ;
            x0 = x0 + t * (x1 - x0);
        }
        // Clip point 1 against near plane
        else if (z1 < nearZ)
        {
            float t = (nearZ - z1) / (z0 - z1);
            z1 = nearZ;
            x1 = x1 + t * (x0 - x1);
        }

        drawWallSegment(fb, x0, z0, x1, z1, sector.floorHeight, sector.ceilingHeight, player.eyeZ, wall.color);
    }
}

void Renderer::drawWallSegment(Framebuffer& fb,
                               float x0, float z0, float x1, float z1,
                               float floorH, float ceilH,
                               float playerEyeZ,
                               uint32_t wallColor)
{
    float halfW = fb.width * 0.5f;
    float halfH = fb.height * 0.5f;
    float focalLen = halfW / std::tan(fov * 0.5f);

    // Project X coordinates to screen
    float sx0 = halfW + (x0 * focalLen) / z0;
    float sx1 = halfW + (x1 * focalLen) / z1;

    // Backface culling: ensure walls are oriented facing the camera
    // If left-to-right on screen is inverted, this face is pointing away
    if (sx0 >= sx1)
    {
        return;
    }

    // Calculate Y projections at endpoints
    float eyeCeil  = playerEyeZ - ceilH;
    float eyeFloor = playerEyeZ - floorH;

    float sy0_top = halfH + (eyeCeil  * focalLen) / z0;
    float sy0_bot = halfH + (eyeFloor * focalLen) / z0;

    float sy1_top = halfH + (eyeCeil  * focalLen) / z1;
    float sy1_bot = halfH + (eyeFloor * focalLen) / z1;

    // Screen clipping bounds
    int startX = std::max(0, static_cast<int>(std::ceil(sx0)));
    int endX   = std::min(fb.width - 1, static_cast<int>(std::floor(sx1)));

    float invSpanX = 1.0f / (sx1 - sx0);

    // Extract wall base RGB components for depth shading
    uint8_t baseR = static_cast<uint8_t>(wallColor & 0xFF);
    uint8_t baseG = static_cast<uint8_t>((wallColor >> 8) & 0xFF);
    uint8_t baseB = static_cast<uint8_t>((wallColor >> 16) & 0xFF);

    // Rasterize vertical columns
    for (int col = startX; col <= endX; ++col)
    {
        float t = (col - sx0) * invSpanX;

        // Interpolate 1/Z for perspective-correct depth
        float invZ = (1.0f - t) * (1.0f / z0) + t * (1.0f / z1);
        float currentZ = 1.0f / invZ;

        // Vertical screen bounds for this column
        float yTopF = (1.0f - t) * sy0_top + t * sy1_top;
        float yBotF = (1.0f - t) * sy0_bot + t * sy1_bot;

        int yTop = std::max(0, static_cast<int>(std::ceil(yTopF)));
        int yBot = std::min(fb.height - 1, static_cast<int>(std::floor(yBotF)));

        // Atmospheric distance shading (dimmer with distance)
        float shade = std::clamp(1.0f - (currentZ / 18.0f), 0.15f, 1.0f);
        uint8_t r = static_cast<uint8_t>(baseR * shade);
        uint8_t g = static_cast<uint8_t>(baseG * shade);
        uint8_t b = static_cast<uint8_t>(baseB * shade);
        uint32_t colColor = Framebuffer::packRGBA(r, g, b, 255);

        // Draw column slice
        for (int y = yTop; y <= yBot; ++y)
        {
            fb.pixels[y * fb.width + col] = colColor;
        }

        // Optional wall column borders (gives an authentic retro panel look)
        if (col == startX || col == endX)
        {
            uint32_t edgeCol = Framebuffer::packRGBA(r / 2, g / 2, b / 2, 255);
            for (int y = yTop; y <= yBot; ++y)
            {
                fb.pixels[y * fb.width + col] = edgeCol;
            }
        }
    }
}
