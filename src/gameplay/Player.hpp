#pragma once
#include "math/Vec2.hpp"

struct Player
{
    Vec2 position{12.0f, 12.0f};
    int currentSector{0};
    float eyeZ{1.6f};
    float angle{1.570796f};
    float moveSpeed{6.0f};
    float mouseSensitivity{0.0025f};

    float walkTimer{0.0f};
    float shootTimer{0.0f};
    int ammo{12};

    Player() = default;
    Player(const Vec2& pos, float ang, float eyeH = 1.6f, int sector = 0);

    void update(float dt);
};
