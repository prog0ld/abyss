#include "core/Application.hpp"
#include <cmath>

void Application::init()
{
    InitWindow(WIN_WIDTH, WIN_HEIGHT, "ABYSS — Sector FPS Engine");
    SetTargetFPS(60);
    DisableCursor();

    textures.init();

    // -------------------------------------------------------------
    // SECTOR 0: Central Laboratory (Start Room, 8x8m, Floor: 0.0, Ceil: 3.0)
    // -------------------------------------------------------------
    Sector lab(0.0f, 3.0f, Framebuffer::rgba(35, 38, 44), Framebuffer::rgba(22, 24, 28));
    // South Wall (solid)
    lab.addWall(Vec2{1.0f, 1.0f}, Vec2{9.0f, 1.0f}, -1, 0);

    // East Wall (leads to Sector 4: Storage between Y: 4..6)
    lab.addWall(Vec2{9.0f, 1.0f}, Vec2{9.0f, 4.0f}, -1, 0);
    lab.addWall(Vec2{9.0f, 4.0f}, Vec2{9.0f, 6.0f},  4, 1); // Portal -> Storage
    lab.addWall(Vec2{9.0f, 6.0f}, Vec2{9.0f, 9.0f}, -1, 0);

    // North Wall (leads to Sector 1: Hallway between X: 4..6)
    lab.addWall(Vec2{9.0f, 9.0f}, Vec2{6.0f, 9.0f}, -1, 0);
    lab.addWall(Vec2{6.0f, 9.0f}, Vec2{4.0f, 9.0f},  1, 1); // Portal -> Hallway
    lab.addWall(Vec2{4.0f, 9.0f}, Vec2{1.0f, 9.0f}, -1, 0);

    // West Wall (leads to Sector 3: Security Office between Y: 4..6)
    lab.addWall(Vec2{1.0f, 9.0f}, Vec2{1.0f, 6.0f}, -1, 0);
    lab.addWall(Vec2{1.0f, 6.0f}, Vec2{1.0f, 4.0f},  3, 1); // Portal -> Security
    lab.addWall(Vec2{1.0f, 4.0f}, Vec2{1.0f, 1.0f}, -1, 0);
    world.addSector(lab);

    // -------------------------------------------------------------
    // SECTOR 1: Maintenance Hallway (2x7m, Floor: 0.3m step, Ceil: 2.4m)
    // -------------------------------------------------------------
    Sector hall(0.3f, 2.4f, Framebuffer::rgba(45, 38, 35), Framebuffer::rgba(28, 22, 20));
    hall.addWall(Vec2{4.0f, 9.0f},  Vec2{6.0f, 9.0f},   0, 1); // Portal back to Lab
    hall.addWall(Vec2{6.0f, 9.0f},  Vec2{6.0f, 16.0f}, -1, 2); // East wall of hall
    hall.addWall(Vec2{6.0f, 16.0f}, Vec2{4.0f, 16.0f},  2, 1); // Portal -> Reactor Core!
    hall.addWall(Vec2{4.0f, 16.0f}, Vec2{4.0f, 9.0f},  -1, 2); // West wall of hall
    world.addSector(hall);

    // -------------------------------------------------------------
    // SECTOR 2: Reactor Generator Core (10x8m, High Ceiling: 4.5m, Floor: 0.0m)
    // -------------------------------------------------------------
    Sector reactor(0.0f, 4.5f, Framebuffer::rgba(25, 32, 40), Framebuffer::rgba(15, 18, 24));
    // South Wall (has portal from hallway at X: 4..6)
    reactor.addWall(Vec2{4.0f, 16.0f},  Vec2{6.0f, 16.0f},  1, 1); // Portal back to Hallway
    reactor.addWall(Vec2{6.0f, 16.0f},  Vec2{10.0f, 16.0f}, -1, 0);
    // East, North, West Walls
    reactor.addWall(Vec2{10.0f, 16.0f}, Vec2{10.0f, 24.0f}, -1, 2);
    reactor.addWall(Vec2{10.0f, 24.0f}, Vec2{0.0f,  24.0f}, -1, 0); // Far reactor wall
    reactor.addWall(Vec2{0.0f,  24.0f}, Vec2{0.0f,  16.0f}, -1, 2);
    reactor.addWall(Vec2{0.0f,  16.0f}, Vec2{4.0f,  16.0f}, -1, 0);
    world.addSector(reactor);

    // -------------------------------------------------------------
    // SECTOR 3: Security & Control Room (5x5m, Floor: 0.0m, Ceil: 2.6m)
    // -------------------------------------------------------------
    Sector security(0.0f, 2.6f, Framebuffer::rgba(30, 35, 30), Framebuffer::rgba(20, 24, 20));
    // East wall connects to Lab
    security.addWall(Vec2{1.0f, 4.0f},  Vec2{1.0f, 6.0f},  0, 1); // Portal back to Lab
    security.addWall(Vec2{1.0f, 6.0f},  Vec2{1.0f, 8.0f}, -1, 0);
    security.addWall(Vec2{1.0f, 8.0f},  Vec2{-4.0f, 8.0f}, -1, 0);
    security.addWall(Vec2{-4.0f, 8.0f}, Vec2{-4.0f, 2.0f}, -1, 2); // West monitoring wall
    security.addWall(Vec2{-4.0f, 2.0f}, Vec2{1.0f, 2.0f},  -1, 0);
    security.addWall(Vec2{1.0f, 2.0f},  Vec2{1.0f, 4.0f}, -1, 0);
    world.addSector(security);

    // -------------------------------------------------------------
    // SECTOR 4: Storage Warehouse (6x6m, Low Floor: -0.2m, Ceil: 3.2m)
    // -------------------------------------------------------------
    Sector storage(-0.2f, 3.2f, Framebuffer::rgba(40, 36, 32), Framebuffer::rgba(24, 22, 20));
    // West wall connects to Lab
    storage.addWall(Vec2{9.0f, 6.0f},  Vec2{9.0f, 4.0f},  0, 1); // Portal back to Lab
    storage.addWall(Vec2{9.0f, 4.0f},  Vec2{9.0f, 2.0f}, -1, 2);
    storage.addWall(Vec2{9.0f, 2.0f},  Vec2{15.0f, 2.0f}, -1, 2);
    storage.addWall(Vec2{15.0f, 2.0f}, Vec2{15.0f, 8.0f}, -1, 0);
    storage.addWall(Vec2{15.0f, 8.0f}, Vec2{9.0f, 8.0f},  -1, 2);
    storage.addWall(Vec2{9.0f, 8.0f},  Vec2{9.0f, 6.0f},  -1, 2);
    world.addSector(storage);

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

    int foundSec = world.findSector(player.position);
    if (foundSec != -1)
    {
        player.currentSector = foundSec;
        player.eyeZ = world.sectors[foundSec].floorHeight + 1.6f;
    }
}

void Application::render()
{
    renderer.render(framebuffer, world, textures, player);
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
        int rx = WIN_WIDTH - 150;
        int ry = 20;
        int rw = 130;
        int rh = 150;
        DrawRectangle(rx, ry, rw, rh, Color{10, 15, 20, 220});
        DrawRectangleLines(rx, ry, rw, rh, DARKGRAY);

        float s = 4.5f;
        float offX = rx + 30;
        float offY = ry + 20;

        for (const auto& sec : world.sectors)
        {
            for (const auto& w : sec.walls)
            {
                Color col = (w.portalSector >= 0) ? Color{50, 200, 255, 200} : Color{160, 160, 160, 255};
                DrawLine((int)(offX + w.p0.x * s), (int)(offY + w.p0.y * s),
                         (int)(offX + w.p1.x * s), (int)(offY + w.p1.y * s), col);
            }
        }

        int px = (int)(offX + player.position.x * s);
        int py = (int)(offY + player.position.y * s);
        DrawCircle(px, py, 3, YELLOW);
        DrawLine(px, py, px + (int)(std::cos(player.angle) * 8.0f), py + (int)(std::sin(player.angle) * 8.0f), YELLOW);

        const char* secNames[] = {
            "Sector 0: Central Laboratory",
            "Sector 1: Maintenance Hall",
            "Sector 2: Generator Core",
            "Sector 3: Security Office",
            "Sector 4: Storage Warehouse"
        };
        int sId = std::clamp(player.currentSector, 0, 4);
        DrawText(secNames[sId], 14, 14, 20, RAYWHITE);
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
