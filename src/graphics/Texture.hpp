#pragma once
#include <vector>
#include <cstdint>
#include <cmath>
#include <algorithm>
#include "graphics/Framebuffer.hpp"

struct WallTexture
{
    int width{64};
    int height{64};
    std::vector<uint32_t> pixels;

    WallTexture() : pixels(64 * 64, Framebuffer::rgba(128, 128, 128)) {}
    WallTexture(int w, int h) : width(w), height(h), pixels(w * h, Framebuffer::rgba(128, 128, 128)) {}

    uint32_t sample(float u, float v) const
    {
        u = u - std::floor(u);
        v = v - std::floor(v);
        int tx = std::clamp(static_cast<int>(u * width), 0, width - 1);
        int ty = std::clamp(static_cast<int>(v * height), 0, height - 1);
        return pixels[ty * width + tx];
    }
};

struct TextureManager
{
    std::vector<WallTexture> textures;

    void init()
    {
        textures.resize(3);

        // Texture 0: Lab Wall Panel (64x64 metal with beveled border and rivets)
        WallTexture& t0 = textures[0];
        for (int y = 0; y < 64; ++y)
        {
            for (int x = 0; x < 64; ++x)
            {
                uint8_t base = 110;
                if (x < 2 || y < 2) base = 145;
                else if (x >= 62 || y >= 62) base = 75;
                if (x == 31 || y == 31) base = 65;

                bool rivet = ((x == 6 || x == 57) && (y == 6 || y == 57));
                if (rivet) base = 210;

                t0.pixels[y * 64 + x] = Framebuffer::rgba(base, base + 8, base + 15);
            }
        }

        // Texture 1: Hazard Stripes (diagonal yellow & dark grey stripes)
        WallTexture& t1 = textures[1];
        for (int y = 0; y < 64; ++y)
        {
            for (int x = 0; x < 64; ++x)
            {
                int stripe = ((x + y) / 8) % 2;
                if (stripe == 0)
                    t1.pixels[y * 64 + x] = Framebuffer::rgba(210, 185, 40);
                else
                    t1.pixels[y * 64 + x] = Framebuffer::rgba(45, 45, 45);
            }
        }

        // Texture 2: Industrial Tiles / Wall Bricks
        WallTexture& t2 = textures[2];
        for (int y = 0; y < 64; ++y)
        {
            for (int x = 0; x < 64; ++x)
            {
                int row = y / 16;
                int shift = (row % 2) * 16;
                int col = (x + shift) % 32;

                uint8_t tone = 120;
                if (y % 16 == 0 || col == 0) tone = 40;
                else tone = 110 + ((x * 3 + y * 7) % 20);

                t2.pixels[y * 64 + x] = Framebuffer::rgba(tone + 5, tone, tone - 5);
            }
        }
    }
};
