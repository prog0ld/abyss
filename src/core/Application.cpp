#include "core/Application.hpp"
#include <cmath>

void Application::init()
{
    InitWindow(WIN_WIDTH, WIN_HEIGHT, "ABYSS — Sector & Portal Engine");
    SetTargetFPS(60);
    DisableCursor();

    // Sector 0: Main Laboratory (8x8m, floor: 0.0m, ceil: 3.0m)
    Sector lab(0.0f, 3.0f, Framebuffer::rgba(35, 38, 44), Framebuffer::rgba(22, 24, 28));
    lab.addWall(Vec2{1.0f, 1.0f}, Vec2{9.0f, 1.0f}, -1, Framebuffer::rgba(100, 110, 120));
    lab.addWall(Vec2{9.0f, 1.0f}, Vec2{9.0f, 9.0f}, -1, Framebuffer::rgba(85, 110, 120));
    
    // North wall of lab with doorway into hallway (portal to Sector 1)
    lab.addWall(Vec2{9.0f, 9.0f}, Vec2{6.0f, 9.0f}, -1, Framebuffer::rgba(120, 110, 85));
    lab.addWall(Vec2{6.0f, 9.0f}, Vec2{4.0f, 9.0f},  1, Framebuffer::rgba(140, 130, 90)); // Portal to Sector 1
    lab.addWall(Vec2{4.0f, 9.0f}, Vec2{1.0f, 9.0f}, -1, Framebuffer::rgba(120, 110, 85));

    lab.addWall(Vec2{1.0f, 9.0f}, Vec2{1.0f, 1.0f}, -1, Framebuffer::rgba(115, 95, 90));
    world.addSector(lab);

    // Sector 1: Maintenance Hallway (2x7m, floor: 0.3m step, ceil: 2.4m)
    Sector hall(0.3f, 2.4f, Framebuffer::rgba(45, 38, 35), Framebuffer::rgba(28, 22, 20));
    hall.addWall(Vec2{4.0f, 9.0f},  Vec2{6.0f, 9.0f},  0, Framebuffer::rgba(140, 130, 90)); // Portal back to Sector 0
    hall.addWall(Vec2{6.0f, 9.0f},  Vec2{6.0f, 16.0f}, -1, Framebuffer::rgba(95, 105, 115));
    hall.addWall(Vec2{6.0f, 16.0f}, Vec2{4.0f, 16.0f}, -1, Framebuffer::rgba(130, 70, 70)); // End of hall (red emergency door)
    hall.addWall(Vec2{4.0f, 16.0f}, Vec2{4.0f, 9.0f},  -1, Framebuffer::rgba(95, 105, 115));
    world.addSector(hall);

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

    // Detect which sector the player is currently inside
    int foundSec = world.findSector(player.position);
    if (foundSec != -1)
    {
        player.currentSector = foundSec;
        player.eyeZ = world.sectors[foundSec].floorHeight + 1.6f;
    }
}

void Application::render()
{
    renderer.render(framebuffer, world, player);
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

        // Minimap / Radar in top-right
        int rx = WIN_WIDTH - 120;
        int ry = 20;
        int rw = 100;
        int rh = 120;
        DrawRectangle(rx, ry, rw, rh, Color{10, 15, 20, 220});
        DrawRectangleLines(rx, ry, rw, rh, DARKGRAY);

        float s = 6.0f;
        for (const auto& sec : world.sectors)
        {
            for (const auto& w : sec.walls)
            {
                Color col = (w.portalSector >= 0) ? Color{50, 200, 255, 200} : Color{180, 180, 180, 255};
                DrawLine(rx + (int)(w.p0.x * s), ry + (int)(w.p0.y * s),
                         rx + (int)(w.p1.x * s), ry + (int)(w.p1.y * s), col);
            }
        }

        int px = rx + (int)(player.position.x * s);
        int py = ry + (int)(player.position.y * s);
        DrawCircle(px, py, 3, YELLOW);
        DrawLine(px, py, px + (int)(std::cos(player.angle) * 10.0f), py + (int)(std::sin(player.angle) * 10.0f), YELLOW);

        const char* secName = (player.currentSector == 0) ? "Sector 0: Main Lab" : "Sector 1: Maintenance Hall";
        DrawText(secName, 14, 14, 20, RAYWHITE);
        DrawFPS(WIN_WIDTH - 80, WIN_HEIGHT - 30);
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
