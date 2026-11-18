#include "core/Application.hpp"
#include <cmath>
#include <string>

Application::Application()
    : framebuffer(FB_WIDTH, FB_HEIGHT)
    , player(Vec2{FB_WIDTH * 0.5f, FB_HEIGHT * 0.5f}, 0.0f)
{
}

Application::~Application()
{
    shutdown();
}

bool Application::init()
{
    InitWindow(WIN_WIDTH, WIN_HEIGHT, "ABYSS — Sector FPS Prototype");
    SetTargetFPS(60);

    // Initial clear
    framebuffer.clear(15, 18, 24);

    // Create GPU Texture2D matching our Framebuffer format
    Image img;
    img.data    = framebuffer.pixels.data();
    img.width   = FB_WIDTH;
    img.height  = FB_HEIGHT;
    img.mipmaps = 1;
    img.format  = PIXELFORMAT_UNCOMPRESSED_R8G8B8A8;

    texture = LoadTextureFromImage(img);
    isRunning = true;
    return true;
}

void Application::update(float dt)
{
    player.update(dt);

    // Keep player inside framebuffer bounds for this prototype
    if (player.position.x < 10.0f) player.position.x = 10.0f;
    if (player.position.x > FB_WIDTH - 10.0f) player.position.x = FB_WIDTH - 10.0f;
    if (player.position.y < 10.0f) player.position.y = 10.0f;
    if (player.position.y > FB_HEIGHT - 10.0f) player.position.y = FB_HEIGHT - 10.0f;
}

void Application::render()
{
    // 1. Clear CPU Framebuffer
    framebuffer.clear(16, 20, 26);

    // 2. Draw test room outline on the software framebuffer
    uint32_t wallColor = Framebuffer::packRGBA(70, 90, 110);
    framebuffer.drawRect(30, 20, FB_WIDTH - 60, 2, wallColor);
    framebuffer.drawRect(30, FB_HEIGHT - 22, FB_WIDTH - 60, 2, wallColor);
    framebuffer.drawRect(30, 20, 2, FB_HEIGHT - 40, wallColor);
    framebuffer.drawRect(FB_WIDTH - 32, 20, 2, FB_HEIGHT - 40, wallColor);

    // 3. Draw Player on the software framebuffer
    int px = static_cast<int>(player.position.x);
    int py = static_cast<int>(player.position.y);
    uint32_t playerColor = Framebuffer::packRGBA(0, 230, 255);

    // Player body (a 5x5 square)
    framebuffer.drawRect(px - 2, py - 2, 5, 5, playerColor);

    // Player direction sight line (16 pixels ahead)
    int targetX = px + static_cast<int>(std::cos(player.angle) * 16.0f);
    int targetY = py + static_cast<int>(std::sin(player.angle) * 16.0f);
    framebuffer.drawLine(px, py, targetX, targetY, Framebuffer::packRGBA(255, 220, 50));

    // 4. Upload Framebuffer to GPU Texture
    UpdateTexture(texture, framebuffer.pixels.data());

    // 5. Present to Screen using Raylib
    Rectangle srcRect  = { 0.0f, 0.0f, static_cast<float>(FB_WIDTH), static_cast<float>(FB_HEIGHT) };
    Rectangle destRect = { 0.0f, 0.0f, static_cast<float>(WIN_WIDTH), static_cast<float>(WIN_HEIGHT) };
    Vector2 origin     = { 0.0f, 0.0f };

    BeginDrawing();
        ClearBackground(BLACK);
        DrawTexturePro(texture, srcRect, destRect, origin, 0.0f, WHITE);

        // Overlay status info
        DrawText("ABYSS: Phase 1 — Player Movement & Framebuffer Verification", 12, 12, 20, RAYWHITE);
        DrawText("Controls: W/S/A/D to Move, Q/E or Left/Right to Rotate view", 12, 38, 18, LIGHTGRAY);

        std::string posText = "Pos: (" + std::to_string(static_cast<int>(player.position.x)) +
                              ", " + std::to_string(static_cast<int>(player.position.y)) +
                              ") | Angle: " + std::to_string(static_cast<int>(player.angle * 180.0f / 3.14159f)) + " deg";
        DrawText(posText.c_str(), 12, 62, 18, YELLOW);

        DrawFPS(WIN_WIDTH - 90, 12);
    EndDrawing();
}

void Application::run()
{
    while (isRunning && !WindowShouldClose())
    {
        float dt = GetFrameTime();
        if (dt > 0.1f) dt = 0.1f; // Clamp delta-time against spiral-of-death on pauses

        update(dt);
        render();
    }
}

void Application::shutdown()
{
    if (isRunning)
    {
        UnloadTexture(texture);
        CloseWindow();
        isRunning = false;
    }
}
