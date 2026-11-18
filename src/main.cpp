//
// ABYSS — Raylib platform proof of concept
//
// Goal: verify the full pipeline from CPU pixel buffer to screen.
//
//   CPU pixel buffer  →  UpdateTexture()  →  DrawTexture()  →  display
//
// What we test here:
//   - Raylib window creation and game loop.
//   - Allocating a pixel buffer (this becomes our software framebuffer).
//   - Painting test colors (5 vertical stripes: red, green, blue, white, black).
//   - Uploading the buffer to a GPU texture every frame.
//   - Verifying that colors appear correctly (channel order check).
//
// Raylib 6.2 pixel format for PIXELFORMAT_UNCOMPRESSED_R8G8B8A8:
//   Each pixel is 4 bytes: R, G, B, A  (in that memory order).
//   We store pixels as uint32_t using the layout:
//       bits [31..24] = A
//       bits [23..16] = B
//       bits [15.. 8] = G
//       bits [ 7.. 0] = R   (little-endian: R is at the lowest address)
//   So the helper below places R in the lowest byte.
//

#include <raylib.h>
#include <vector>
#include <cstdint>

// -------------------------------------------------------------------------
// Framebuffer dimensions.
// Using half the window size for a classic software-rendered look.
// The texture is scaled up to fill the 960x540 window.
// -------------------------------------------------------------------------
static constexpr int FB_WIDTH  = 480;
static constexpr int FB_HEIGHT = 270;

static constexpr int WIN_WIDTH  = 960;
static constexpr int WIN_HEIGHT = 540;

// -------------------------------------------------------------------------
// Pack R, G, B, A into a uint32_t in Raylib's expected R8G8B8A8 order.
// On a little-endian system, the bytes in memory are [R, G, B, A].
// -------------------------------------------------------------------------
static constexpr uint32_t packRGBA(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255)
{
    return (static_cast<uint32_t>(a) << 24)
         | (static_cast<uint32_t>(b) << 16)
         | (static_cast<uint32_t>(g) <<  8)
         | (static_cast<uint32_t>(r));
}

// -------------------------------------------------------------------------
// Fill the framebuffer with five vertical color stripes.
// This is our channel-order sanity check.
// -------------------------------------------------------------------------
static void drawTestPattern(std::vector<uint32_t>& pixels, int width, int height)
{
    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            uint32_t color;
            int stripe = (x * 5) / width;   // 0..4

            switch (stripe)
            {
                case 0:  color = packRGBA(255,   0,   0); break; // red
                case 1:  color = packRGBA(  0, 255,   0); break; // green
                case 2:  color = packRGBA(  0,   0, 255); break; // blue
                case 3:  color = packRGBA(255, 255, 255); break; // white
                default: color = packRGBA(  0,   0,   0); break; // black
            }

            pixels[y * width + x] = color;
        }
    }
}

int main()
{
    // Initialize Raylib window.
    InitWindow(WIN_WIDTH, WIN_HEIGHT, "ABYSS — Raylib PoC");
    SetTargetFPS(60);

    // -------------------------------------------------------------------------
    // Allocate the CPU-side pixel buffer.
    // This vector is the future ABYSS software framebuffer.
    // -------------------------------------------------------------------------
    std::vector<uint32_t> pixels(FB_WIDTH * FB_HEIGHT);

    // Fill once with the test pattern.
    drawTestPattern(pixels, FB_WIDTH, FB_HEIGHT);

    // -------------------------------------------------------------------------
    // Create a Raylib Image from our pixel buffer, then upload it to the GPU
    // as a Texture2D.
    //
    // Image.data points to our pixels — we do NOT take ownership; Raylib
    // treats it as read-only source data for LoadTextureFromImage().
    // After that call, the GPU texture is independent and we own the buffer.
    // -------------------------------------------------------------------------
    Image img;
    img.data    = pixels.data();
    img.width   = FB_WIDTH;
    img.height  = FB_HEIGHT;
    img.mipmaps = 1;
    img.format  = PIXELFORMAT_UNCOMPRESSED_R8G8B8A8;

    Texture2D texture = LoadTextureFromImage(img);
    // img.data was not heap-allocated by Raylib, so we do NOT call UnloadImage().

    // -------------------------------------------------------------------------
    // Rectangle defining where to draw the texture on screen.
    // We scale from (FB_WIDTH x FB_HEIGHT) up to (WIN_WIDTH x WIN_HEIGHT).
    // -------------------------------------------------------------------------
    Rectangle srcRect  = { 0, 0, (float)FB_WIDTH, (float)FB_HEIGHT };
    Rectangle destRect = { 0, 0, (float)WIN_WIDTH, (float)WIN_HEIGHT };
    Vector2   origin   = { 0, 0 };

    // Main loop.
    while (!WindowShouldClose())
    {
        // -----------------------------------------------------------------
        // Update: in future phases this is where the renderer writes pixels.
        // For now we just re-upload the static test pattern every frame to
        // confirm that UpdateTexture() works correctly.
        // -----------------------------------------------------------------
        UpdateTexture(texture, pixels.data());

        // -----------------------------------------------------------------
        // Draw.
        // -----------------------------------------------------------------
        BeginDrawing();
            ClearBackground(BLACK);

            // Draw the framebuffer texture scaled to window size.
            // TEXTURE_FILTER_POINT = nearest-neighbor (classic pixel look).
            DrawTexturePro(texture, srcRect, destRect, origin, 0.0f, WHITE);

            DrawFPS(8, 8);
        EndDrawing();
    }

    // Cleanup.
    UnloadTexture(texture);
    CloseWindow();

    return 0;
}
