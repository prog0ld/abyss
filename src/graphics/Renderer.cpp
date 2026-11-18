#include "graphics/Renderer.hpp"
#include <cmath>
#include <algorithm>

void Renderer::render(Framebuffer& fb, const Sector& sector, const Player& player)
{
    int halfH = fb.height / 2;
    for (int y = 0; y < halfH; ++y)
        for (int x = 0; x < fb.width; ++x)
            fb.pixels[y * fb.width + x] = sector.ceilingColor;

    for (int y = halfH; y < fb.height; ++y)
        for (int x = 0; x < fb.width; ++x)
            fb.pixels[y * fb.width + x] = sector.floorColor;

    float cosA = std::cos(player.angle);
    float sinA = std::sin(player.angle);

    float halfW = fb.width * 0.5f;
    float focalLen = halfW / std::tan(fov * 0.5f);

    for (const auto& wall : sector.walls)
    {
        float dx0 = wall.p0.x - player.position.x;
        float dy0 = wall.p0.y - player.position.y;
        float dx1 = wall.p1.x - player.position.x;
        float dy1 = wall.p1.y - player.position.y;

        float z0 = dx0 * cosA + dy0 * sinA;
        float x0 = -dx0 * sinA + dy0 * cosA;

        float z1 = dx1 * cosA + dy1 * sinA;
        float x1 = -dx1 * sinA + dy1 * cosA;

        if (z0 <= nearZ && z1 <= nearZ)
            continue;

        if (z0 < nearZ)
        {
            float t = (nearZ - z0) / (z1 - z0);
            z0 = nearZ;
            x0 = x0 + t * (x1 - x0);
        }
        else if (z1 < nearZ)
        {
            float t = (nearZ - z1) / (z0 - z1);
            z1 = nearZ;
            x1 = x1 + t * (x0 - x1);
        }

        float sx0 = halfW + (x0 * focalLen) / z0;
        float sx1 = halfW + (x1 * focalLen) / z1;

        if (sx0 >= sx1)
            continue;

        float eyeCeil = player.eyeZ - sector.ceilingHeight;
        float eyeFloor = player.eyeZ - sector.floorHeight;

        float sy0_top = halfH + (eyeCeil * focalLen) / z0;
        float sy0_bot = halfH + (eyeFloor * focalLen) / z0;
        float sy1_top = halfH + (eyeCeil * focalLen) / z1;
        float sy1_bot = halfH + (eyeFloor * focalLen) / z1;

        int startX = std::max(0, static_cast<int>(std::ceil(sx0)));
        int endX = std::min(fb.width - 1, static_cast<int>(std::floor(sx1)));

        float invSpan = 1.0f / (sx1 - sx0);

        uint8_t baseR = static_cast<uint8_t>(wall.color & 0xFF);
        uint8_t baseG = static_cast<uint8_t>((wall.color >> 8) & 0xFF);
        uint8_t baseB = static_cast<uint8_t>((wall.color >> 16) & 0xFF);

        for (int col = startX; col <= endX; ++col)
        {
            float t = (col - sx0) * invSpan;
            float z = 1.0f / ((1.0f - t) / z0 + t / z1);

            int yTop = std::max(0, static_cast<int>(std::ceil((1.0f - t) * sy0_top + t * sy1_top)));
            int yBot = std::min(fb.height - 1, static_cast<int>(std::floor((1.0f - t) * sy0_bot + t * sy1_bot)));

            float shade = std::clamp(1.0f - (z / 18.0f), 0.2f, 1.0f);
            uint32_t colColor = Framebuffer::rgba(
                static_cast<uint8_t>(baseR * shade),
                static_cast<uint8_t>(baseG * shade),
                static_cast<uint8_t>(baseB * shade)
            );

            for (int y = yTop; y <= yBot; ++y)
            {
                fb.pixels[y * fb.width + col] = colColor;
            }
        }
    }
}
