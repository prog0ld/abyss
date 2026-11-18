#include "core/Application.hpp"
#include <cmath>

void Application::init()
{
    InitWindow(WIN_WIDTH, WIN_HEIGHT, "ABYSS");
    SetTargetFPS(60);
    DisableCursor();

    room = Sector(0.0f, 3.0f, Framebuffer::rgba(35, 38, 42), Framebuffer::rgba(20, 22, 25));
    room.addWall(Vec2{1.0f, 1.0f}, Vec2{9.0f, 1.0f}, Framebuffer::rgba(100, 110, 120));
    room.addWall(Vec2{9.0f, 1.0f}, Vec2{9.0f, 9.0f}, Framebuffer::rgba(80, 110, 120));
    room.addWall(Vec2{9.0f, 9.0f}, Vec2{1.0f, 9.0f}, Framebuffer::rgba(120, 110, 80));
    room.addWall(Vec2{1.0f, 9.0f}, Vec2{1.0f, 1.0f}, Framebuffer::rgba(120, 90, 85));

    Image img;
    img.data = framebuffer.pixels.data();
    img.width = FB_WIDTH;
    img.height = FB_HEIGHT;
    img.mipmaps = 1;
    img.format = PIXELFORMAT_UNCOMPRESSED_R8G8B8A8;

    texture = LoadTextureFromImage(img);
}

void Application::update(float dt)
{
    if (IsKeyPressed(KEY_TAB))
    {
        if (IsCursorHidden()) EnableCursor();
        else DisableCursor();
    }

    if (IsCursorHidden())
    {
        player.update(dt);
    }

    if (player.position.x < 1.3f) player.position.x = 1.3f;
    if (player.position.x > 8.7f) player.position.x = 8.7f;
    if (player.position.y < 1.3f) player.position.y = 1.3f;
    if (player.position.y > 8.7f) player.position.y = 8.7f;
}

void Application::render()
{
    renderer.render(framebuffer, room, player);
    UpdateTexture(texture, framebuffer.pixels.data());

    Rectangle src = {0.0f, 0.0f, (float)FB_WIDTH, (float)FB_HEIGHT};
    Rectangle dst = {0.0f, 0.0f, (float)WIN_WIDTH, (float)WIN_HEIGHT};

    BeginDrawing();
        ClearBackground(BLACK);
        DrawTexturePro(texture, src, dst, {0.0f, 0.0f}, 0.0f, WHITE);

        int cx = WIN_WIDTH / 2;
        int cy = WIN_HEIGHT / 2;
        DrawLine(cx - 5, cy, cx + 5, cy, GREEN);
        DrawLine(cx, cy - 5, cx, cy + 5, GREEN);

        DrawFPS(10, 10);
    EndDrawing();
}

void Application::run()
{
    init();
    while (!WindowShouldClose())
    {
        update(GetFrameTime());
        render();
    }
    shutdown();
}

void Application::shutdown()
{
    UnloadTexture(texture);
    CloseWindow();
}
