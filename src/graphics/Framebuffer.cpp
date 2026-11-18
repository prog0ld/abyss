#include "graphics/Framebuffer.hpp"
#include <algorithm>
#include <cmath>

Framebuffer::Framebuffer(int w, int h)
{
    resize(w, h);
}

void Framebuffer::resize(int w, int h)
{
    width = w;
    height = h;
    pixels.assign(static_cast<size_t>(w * h), packRGBA(0, 0, 0, 255));
}

void Framebuffer::clear(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
    uint32_t color = packRGBA(r, g, b, a);
    std::fill(pixels.begin(), pixels.end(), color);
}

void Framebuffer::setPixel(int x, int y, uint32_t color)
{
    if (x >= 0 && x < width && y >= 0 && y < height)
    {
        pixels[static_cast<size_t>(y * width + x)] = color;
    }
}

void Framebuffer::setPixel(int x, int y, uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
    setPixel(x, y, packRGBA(r, g, b, a));
}

void Framebuffer::drawLine(int x0, int y0, int x1, int y1, uint32_t color)
{
    // Bresenham's line algorithm
    int dx = std::abs(x1 - x0);
    int dy = -std::abs(y1 - y0);
    int sx = (x0 < x1) ? 1 : -1;
    int sy = (y0 < y1) ? 1 : -1;
    int err = dx + dy;

    while (true)
    {
        setPixel(x0, y0, color);
        if (x0 == x1 && y0 == y1) break;
        int e2 = 2 * err;
        if (e2 >= dy)
        {
            err += dy;
            x0 += sx;
        }
        if (e2 <= dx)
        {
            err += dx;
            y0 += sy;
        }
    }
}

void Framebuffer::drawRect(int x, int y, int w, int h, uint32_t color)
{
    for (int row = y; row < y + h; ++row)
    {
        for (int col = x; col < x + w; ++col)
        {
            setPixel(col, row, color);
        }
    }
}
