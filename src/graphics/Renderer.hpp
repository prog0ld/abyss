#pragma once
#include "graphics/Framebuffer.hpp"
#include "world/Sector.hpp"
#include "gameplay/Player.hpp"

struct Renderer
{
    float fov{1.04719755f};
    float nearZ{0.05f};

    void render(Framebuffer& fb, const Sector& sector, const Player& player);
};
