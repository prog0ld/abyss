#include "graphics/Renderer.hpp"
#include <cmath>
#include <algorithm>

void Renderer::render(Framebuffer& fb, const World& world, const Player& player)
{
    fb.clear(10, 12, 16);

    std::vector<int> topClip(fb.width, 0);
    std::vector<int> botClip(fb.width, fb.height - 1);

    int startSector = player.currentSector;
    if (startSector < 0 || startSector >= static_cast<int>(world.sectors.size()))
    {
        startSector = 0;
    }

    renderSector(fb, world, player, startSector, 0, fb.width - 1, topClip, botClip, 0);
}

void Renderer::renderSector(Framebuffer& fb, const World& world, const Player& player,
                            int sectorId, int startCol, int endCol,
                            std::vector<int>& topClip, std::vector<int>& botClip, int depth)
{
    if (depth > 16 || startCol > endCol)
        return;

    const Sector& sector = world.sectors[sectorId];

    float cosA = std::cos(player.angle);
    float sinA = std::sin(player.angle);
    float halfW = fb.width * 0.5f;
    float halfH = fb.height * 0.5f;
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

        int wallStart = std::max(startCol, static_cast<int>(std::ceil(sx0)));
        int wallEnd   = std::min(endCol,   static_cast<int>(std::floor(sx1)));
        if (wallStart > wallEnd)
            continue;

        float eyeCeil  = player.eyeZ - sector.ceilingHeight;
        float eyeFloor = player.eyeZ - sector.floorHeight;

        float sy0_ceil  = halfH + (eyeCeil  * focalLen) / z0;
        float sy0_floor = halfH + (eyeFloor * focalLen) / z0;
        float sy1_ceil  = halfH + (eyeCeil  * focalLen) / z1;
        float sy1_floor = halfH + (eyeFloor * focalLen) / z1;

        float n_sy0_ceil  = sy0_ceil;
        float n_sy0_floor = sy0_floor;
        float n_sy1_ceil  = sy1_ceil;
        float n_sy1_floor = sy1_floor;

        bool isPortal = (wall.portalSector >= 0 && wall.portalSector < static_cast<int>(world.sectors.size()));
        if (isPortal)
        {
            const Sector& neighbor = world.sectors[wall.portalSector];
            float nEyeCeil  = player.eyeZ - neighbor.ceilingHeight;
            float nEyeFloor = player.eyeZ - neighbor.floorHeight;

            n_sy0_ceil  = halfH + (nEyeCeil  * focalLen) / z0;
            n_sy0_floor = halfH + (nEyeFloor * focalLen) / z0;
            n_sy1_ceil  = halfH + (nEyeCeil  * focalLen) / z1;
            n_sy1_floor = halfH + (nEyeFloor * focalLen) / z1;
        }

        float invSpan = 1.0f / (sx1 - sx0);
        uint8_t baseR = static_cast<uint8_t>(wall.color & 0xFF);
        uint8_t baseG = static_cast<uint8_t>((wall.color >> 8) & 0xFF);
        uint8_t baseB = static_cast<uint8_t>((wall.color >> 16) & 0xFF);

        std::vector<int> portalTop(fb.width);
        std::vector<int> portalBot(fb.width);

        for (int col = wallStart; col <= wallEnd; ++col)
        {
            float t = (col - sx0) * invSpan;
            float z = 1.0f / ((1.0f - t) / z0 + t / z1);

            int cTop = static_cast<int>(std::ceil((1.0f - t) * sy0_ceil  + t * sy1_ceil));
            int cBot = static_cast<int>(std::floor((1.0f - t) * sy0_floor + t * sy1_floor));

            int curTop = topClip[col];
            int curBot = botClip[col];

            for (int y = curTop; y < std::min(cTop, curBot + 1); ++y)
                fb.pixels[y * fb.width + col] = sector.ceilingColor;

            for (int y = std::max(cBot + 1, curTop); y <= curBot; ++y)
                fb.pixels[y * fb.width + col] = sector.floorColor;

            float shade = std::clamp(1.0f - (z / 20.0f), 0.2f, 1.0f);
            uint32_t colColor = Framebuffer::rgba(
                static_cast<uint8_t>(baseR * shade),
                static_cast<uint8_t>(baseG * shade),
                static_cast<uint8_t>(baseB * shade)
            );

            if (!isPortal)
            {
                int drawTop = std::clamp(cTop, curTop, curBot);
                int drawBot = std::clamp(cBot, curTop, curBot);
                for (int y = drawTop; y <= drawBot; ++y)
                    fb.pixels[y * fb.width + col] = colColor;

                topClip[col] = fb.height;
            }
            else
            {
                int ncTop = static_cast<int>(std::ceil((1.0f - t) * n_sy0_ceil  + t * n_sy1_ceil));
                int ncBot = static_cast<int>(std::floor((1.0f - t) * n_sy0_floor + t * n_sy1_floor));

                int stepTop = std::clamp(cTop, curTop, curBot);
                int stepCeilBot = std::clamp(ncTop - 1, curTop, curBot);
                for (int y = stepTop; y <= stepCeilBot; ++y)
                    fb.pixels[y * fb.width + col] = colColor;

                int stepFloorTop = std::clamp(ncBot + 1, curTop, curBot);
                int stepBot = std::clamp(cBot, curTop, curBot);
                for (int y = stepFloorTop; y <= stepBot; ++y)
                    fb.pixels[y * fb.width + col] = colColor;

                portalTop[col] = std::clamp(std::max(cTop, ncTop), curTop, curBot);
                portalBot[col] = std::clamp(std::min(cBot, ncBot), curTop, curBot);
            }
        }

        if (isPortal)
        {
            std::vector<int> subTop = topClip;
            std::vector<int> subBot = botClip;
            for (int col = wallStart; col <= wallEnd; ++col)
            {
                subTop[col] = portalTop[col];
                subBot[col] = portalBot[col];
            }

            renderSector(fb, world, player, wall.portalSector, wallStart, wallEnd, subTop, subBot, depth + 1);

            for (int col = wallStart; col <= wallEnd; ++col)
            {
                topClip[col] = subTop[col];
            }
        }
    }
}
