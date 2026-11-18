#include "core/Application.hpp"
#include <cmath>
#include <string>

Application::Application()
    : framebuffer(FB_WIDTH, FB_HEIGHT)
    , player(Vec2{5.0f, 5.0f}, 1.570796f, 1.6f) // Centered in room, facing forward
{
}

Application::~Application()
{
    shutdown();
}

void Application::setupWorld()
{
    // A 10x10m underground laboratory sector
    // Floor at 0.0m, Ceiling at 3.0m
    testRoom = Sector(0.0f, 3.0f,
                      Framebuffer::packRGBA(38, 42, 48),  // Dark concrete floor
                      Framebuffer::packRGBA(20, 22, 26)); // Industrial ceiling

    // 4 Walls oriented clockwise around room perimeter:
    // South wall
    testRoom.addWall(Vec2{1.0f, 1.0f}, Vec2{9.0f, 1.0f}, -1, Framebuffer::packRGBA(105, 115, 125));
    // East wall
    testRoom.addWall(Vec2{9.0f, 1.0f}, Vec2{9.0f, 9.0f}, -1, Framebuffer::packRGBA(85, 115, 125));
    // North wall
    testRoom.addWall(Vec2{9.0f, 9.0f}, Vec2{1.0f, 9.0f}, -1, Framebuffer::packRGBA(125, 115, 85));
    // West wall
    testRoom.addWall(Vec2{1.0f, 9.0f}, Vec2{1.0f, 1.0f}, -1, Framebuffer::packRGBA(125, 95, 90));
}

bool Application::init()
{
    InitWindow(WIN_WIDTH, WIN_HEIGHT, "ABYSS — 2.5D Sector Engine");
    SetTargetFPS(60);

    // Capture and hide mouse cursor for standard FPS controls
    DisableCursor();

    setupWorld();

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
    // Toggle cursor capture with TAB or ESCAPE
    if (IsKeyPressed(KEY_TAB))
    {
        if (IsCursorHidden())
        {
            EnableCursor();
        }
        else
        {
            DisableCursor();
        }
    }

    if (IsCursorHidden())
    {
        player.update(dt);
    }

    // Basic wall collision constraint (keep player within room boundaries)
    float minB = 1.3f;
    float maxB = 8.7f;
    if (player.position.x < minB) player.position.x = minB;
    if (player.position.x > maxB) player.position.x = maxB;
    if (player.position.y < minB) player.position.y = minB;
    if (player.position.y > maxB) player.position.y = maxB;
}

void Application::render()
{
    // 1. Render 3D first-person perspective into CPU software framebuffer!
    renderer.render(framebuffer, testRoom, player);

    // 2. Upload Framebuffer to GPU Texture
    UpdateTexture(texture, framebuffer.pixels.data());

    // 3. Present to Screen using Raylib
    Rectangle srcRect  = { 0.0f, 0.0f, static_cast<float>(FB_WIDTH), static_cast<float>(FB_HEIGHT) };
    Rectangle destRect = { 0.0f, 0.0f, static_cast<float>(WIN_WIDTH), static_cast<float>(WIN_HEIGHT) };
    Vector2 origin     = { 0.0f, 0.0f };

    BeginDrawing();
        ClearBackground(BLACK);
        // Draw 3D software-rendered world scaled to window
        DrawTexturePro(texture, srcRect, destRect, origin, 0.0f, WHITE);

        // Crosshair in center
        int cx = WIN_WIDTH / 2;
        int cy = WIN_HEIGHT / 2;
        DrawLine(cx - 6, cy, cx + 6, cy, Color{0, 255, 200, 180});
        DrawLine(cx, cy - 6, cx, cy + 6, Color{0, 255, 200, 180});

        // Debug & Learning HUD
        DrawText("ABYSS: Phase 4 — First 3D Sector Wall Projection", 14, 14, 20, RAYWHITE);
        DrawText("Mouse: Look  |  W/S/A/D: Move & Strafe  |  TAB: Lock/Free Mouse", 14, 38, 16, LIGHTGRAY);

        int deg = static_cast<int>(player.angle * 180.0f / 3.14159f) % 360;
        if (deg < 0) deg += 360;
        std::string posText = "Pos: (" + std::to_string(player.position.x).substr(0, 4) +
                              ", " + std::to_string(player.position.y).substr(0, 4) +
                              ") | Yaw: " + std::to_string(deg) + " deg";
        DrawText(posText.c_str(), 14, 60, 16, YELLOW);

        // Mini 2D Top-Down Radar in top-right corner
        int radarX = WIN_WIDTH - 120;
        int radarY = 20;
        int radarSize = 100;
        DrawRectangle(radarX, radarY, radarSize, radarSize, Color{10, 15, 20, 200});
        DrawRectangleLines(radarX, radarY, radarSize, radarSize, DARKGRAY);

        // Draw room boundary on radar
        float rScale = radarSize / 10.0f;
        for (const auto& w : testRoom.walls)
        {
            DrawLine(radarX + static_cast<int>(w.p0.x * rScale),
                     radarY + static_cast<int>(w.p0.y * rScale),
                     radarX + static_cast<int>(w.p1.x * rScale),
                     radarY + static_cast<int>(w.p1.y * rScale),
                     GRAY);
        }

        // Draw player dot and view cone on radar
        int rpx = radarX + static_cast<int>(player.position.x * rScale);
        int rpy = radarY + static_cast<int>(player.position.y * rScale);
        DrawCircle(rpx, rpy, 3, Color{0, 230, 255, 255});
        DrawLine(rpx, rpy,
                 rpx + static_cast<int>(std::cos(player.angle) * 12.0f),
                 rpy + static_cast<int>(std::sin(player.angle) * 12.0f),
                 YELLOW);

        DrawFPS(WIN_WIDTH - 80, WIN_HEIGHT - 30);
    EndDrawing();
}

void Application::run()
{
    while (isRunning && !WindowShouldClose())
    {
        float dt = GetFrameTime();
        if (dt > 0.1f) dt = 0.1f;

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
