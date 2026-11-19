#include "gameplay/Player.hpp"
#include <raylib.h>
#include <cmath>

Player::Player(const Vec2& pos, float ang, float eyeH, int sector)
    : position(pos), currentSector(sector), eyeZ(eyeH), angle(ang)
{
}

void Player::update(float dt)
{
    Vector2 md = GetMouseDelta();
    angle += md.x * mouseSensitivity;

    Vec2 forward{std::cos(angle), std::sin(angle)};
    Vec2 right{-std::sin(angle), std::cos(angle)};

    Vec2 move{0.0f, 0.0f};
    if (IsKeyDown(KEY_W)) move += forward;
    if (IsKeyDown(KEY_S)) move -= forward;
    if (IsKeyDown(KEY_D)) move += right;
    if (IsKeyDown(KEY_A)) move -= right;

    if (move.length() > 0.0001f)
    {
        position += move.normalized() * (moveSpeed * dt);
        walkTimer += dt * 10.0f;
    }
    else
    {
        walkTimer = 0.0f;
    }

    if (shootTimer > 0.0f)
    {
        shootTimer -= dt;
    }
}
