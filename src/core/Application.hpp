#pragma once
#include <raylib.h>
#include "graphics/Framebuffer.hpp"
#include "graphics/Renderer.hpp"
#include "graphics/Texture.hpp"
#include "world/World.hpp"
#include "world/Door.hpp"
#include "gameplay/Player.hpp"

struct Application
{
    static constexpr int WIN_WIDTH  = 960;
    static constexpr int WIN_HEIGHT = 540;
    static constexpr int FB_WIDTH   = 480;
    static constexpr int FB_HEIGHT  = 270;

    Framebuffer framebuffer{FB_WIDTH, FB_HEIGHT};
    Texture2D texture{};
    TextureManager textures;
    Renderer renderer;
    World world;
    Door securityDoor;
    Player player{Vec2{12.0f, 12.0f}, 1.57f, 1.6f, 0};
    bool canInteractDoor{false};

    void init();
    void run();
    void update(float dt);
    void render();
    void shutdown();

private:
    void handleCollisions();
    void drawWeaponHUD();
};
