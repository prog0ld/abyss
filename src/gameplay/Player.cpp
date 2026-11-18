#include "gameplay/Player.hpp"
#include <raylib.h>
#include <cmath>

Player::Player(const Vec2& startPos, float startAngle)
    : position(startPos), angle(startAngle)
{
}

void Player::update(float dt)
{
    // Turn controls (A / D or Left / Right arrow keys)
    if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_Q))
    {
        angle -= turnSpeed * dt;
    }
    if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_E))
    {
        angle += turnSpeed * dt;
    }

    // Forward and right unit direction vectors
    Vec2 forward{std::cos(angle), std::sin(angle)};
    Vec2 right{-std::sin(angle), std::cos(angle)};

    Vec2 moveDir{0.0f, 0.0f};

    if (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP))
    {
        moveDir += forward;
    }
    if (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN))
    {
        moveDir -= forward;
    }
    if (IsKeyDown(KEY_D))
    {
        moveDir += right;
    }
    if (IsKeyDown(KEY_A))
    {
        moveDir -= right;
    }

    if (moveDir.lengthSq() > 0.0001f)
    {
        position += moveDir.normalized() * (moveSpeed * dt);
    }
}
