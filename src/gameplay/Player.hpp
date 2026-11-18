#pragma once
#include "math/Vec2.hpp"

struct Player
{
    Vec2 position{5.0f, 5.0f};
    float eyeZ{1.6f};        // Eye height above ground in world units
    float angle{0.0f};       // Radians: 0 points along +X, pi/2 along +Y
    float moveSpeed{4.0f};   // Units per second
    float turnSpeed{2.5f};   // Radians per second

    Player() = default;
    Player(const Vec2& startPos, float startAngle, float eyeHeight = 1.6f);

    void update(float dt);
};
