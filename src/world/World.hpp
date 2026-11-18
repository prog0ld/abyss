#pragma once
#include "world/Sector.hpp"
#include <vector>

struct World
{
    std::vector<Sector> sectors;

    void addSector(const Sector& sector)
    {
        sectors.push_back(sector);
    }

    int findSector(const Vec2& pos) const
    {
        for (size_t s = 0; s < sectors.size(); ++s)
        {
            const auto& walls = sectors[s].walls;
            bool inside = true;
            for (const auto& w : walls)
            {
                float cross = (w.p1.x - w.p0.x) * (pos.y - w.p0.y) - (w.p1.y - w.p0.y) * (pos.x - w.p0.x);
                if (cross < 0.0f)
                {
                    inside = false;
                    break;
                }
            }
            if (inside) return static_cast<int>(s);
        }
        return -1;
    }
};
