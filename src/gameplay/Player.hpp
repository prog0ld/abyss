#pragma once
#include "math/Vec2.hpp"

struct Player
{
    Vec2 position{5.0f, 5.0f};
    float eyeZ{1.6f};
    float angle{1.570796f};
    float moveSpeed{4.0f};
    float mouseSensitivity{0.0025f};

    Player() = default;
    Player(const Vec2& pos, float ang, float eyeH = 1.6f);

    void update(float dt);
};
