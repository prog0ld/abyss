#pragma once
#include "math/Vec2.hpp"

struct Player
{
    Vec2 position{240.0f, 135.0f};
    float angle{0.0f}; // Radians: 0 points along +X, pi/2 along +Y
    float moveSpeed{120.0f}; // Units per second
    float turnSpeed{3.0f};   // Radians per second

    Player() = default;
    Player(const Vec2& startPos, float startAngle);

    void update(float dt);
};
