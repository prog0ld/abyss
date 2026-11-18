#pragma once
#include <vector>
#include <cstdint>

struct Framebuffer
{
    int width{0};
    int height{0};
    std::vector<uint32_t> pixels;

    Framebuffer() = default;
    Framebuffer(int w, int h);

    void clear(uint8_t r, uint8_t g, uint8_t b);
    void setPixel(int x, int y, uint32_t color);

    static constexpr uint32_t rgba(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255)
    {
        return (static_cast<uint32_t>(a) << 24) |
               (static_cast<uint32_t>(b) << 16) |
               (static_cast<uint32_t>(g) << 8)  |
               (static_cast<uint32_t>(r));
    }
};
