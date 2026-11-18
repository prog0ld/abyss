#pragma once
#include <raylib.h>
#include "graphics/Framebuffer.hpp"
#include "gameplay/Player.hpp"

struct Application
{
    static constexpr int WIN_WIDTH  = 960;
    static constexpr int WIN_HEIGHT = 540;
    static constexpr int FB_WIDTH   = 480;
    static constexpr int FB_HEIGHT  = 270;

    Framebuffer framebuffer;
    Texture2D texture{};
    Player player;
    bool isRunning{false};

    Application();
    ~Application();

    bool init();
    void run();
    void update(float dt);
    void render();
    void shutdown();
};
