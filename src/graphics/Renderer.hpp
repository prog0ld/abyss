#pragma once
#include "graphics/Framebuffer.hpp"
#include "graphics/Texture.hpp"
#include "world/World.hpp"
#include "gameplay/Player.hpp"
#include <vector>

struct Renderer
{
    float fov{1.04719755f};
    float nearZ{0.05f};

    void render(Framebuffer& fb, const World& world, const TextureManager& textures, const Player& player);

private:
    void renderSector(Framebuffer& fb, const World& world, const TextureManager& textures, const Player& player,
                      int sectorId, int startCol, int endCol,
                      std::vector<int>& topClip, std::vector<int>& botClip, int depth);
};
