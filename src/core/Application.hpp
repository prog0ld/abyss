#pragma once
#include <raylib.h>
#include "graphics/Framebuffer.hpp"
#include "graphics/Renderer.hpp"
#include "world/Sector.hpp"
#include "gameplay/Player.hpp"

struct Application
{
    static constexpr int WIN_WIDTH  = 960;
    static constexpr int WIN_HEIGHT = 540;
    static constexpr int FB_WIDTH   = 480;
    static constexpr int FB_HEIGHT  = 270;

    Framebuffer framebuffer{FB_WIDTH, FB_HEIGHT};
    Texture2D texture{};
    Renderer renderer;
    Sector room;
    Player player{Vec2{5.0f, 5.0f}, 1.57f, 1.6f};

    void init();
    void run();
    void update(float dt);
    void render();
    void shutdown();
};
