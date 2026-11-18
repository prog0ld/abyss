#pragma once
#include <cmath>

struct Vec2
{
    float x{0.0f};
    float y{0.0f};

    Vec2() = default;
    Vec2(float inX, float inY) : x(inX), y(inY) {}

    Vec2 operator+(const Vec2& o) const { return {x + o.x, y + o.y}; }
    Vec2 operator-(const Vec2& o) const { return {x - o.x, y - o.y}; }
    Vec2 operator*(float s) const { return {x * s, y * s}; }

    Vec2& operator+=(const Vec2& o) { x += o.x; y += o.y; return *this; }
    Vec2& operator-=(const Vec2& o) { x -= o.x; y -= o.y; return *this; }

    float length() const { return std::sqrt(x * x + y * y); }

    Vec2 normalized() const
    {
        float l = length();
        return (l > 0.0001f) ? Vec2{x / l, y / l} : Vec2{0.0f, 0.0f};
    }
};
