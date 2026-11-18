#include "graphics/Framebuffer.hpp"
#include <algorithm>

Framebuffer::Framebuffer(int w, int h)
    : width(w), height(h), pixels(static_cast<size_t>(w * h), rgba(0, 0, 0))
{
}

void Framebuffer::clear(uint8_t r, uint8_t g, uint8_t b)
{
    uint32_t c = rgba(r, g, b);
    std::fill(pixels.begin(), pixels.end(), c);
}

void Framebuffer::setPixel(int x, int y, uint32_t color)
{
    if (x >= 0 && x < width && y >= 0 && y < height)
    {
        pixels[static_cast<size_t>(y * width + x)] = color;
    }
}
