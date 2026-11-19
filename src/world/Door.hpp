#pragma once
#include <algorithm>

struct Door
{
    int sectorId{-1};
    float minCeil{0.0f};
    float maxCeil{2.8f};
    float currentCeil{0.0f};
    float speed{2.0f}; // meters per second
    bool isOpening{false};
    bool isOpen{false};

    Door() = default;
    Door(int secId, float minC, float maxC, float spd = 2.0f)
        : sectorId(secId), minCeil(minC), maxCeil(maxC), currentCeil(minC), speed(spd)
    {
    }

    void open()
    {
        if (!isOpen)
        {
            isOpening = true;
        }
    }

    void update(float dt)
    {
        if (isOpening && !isOpen)
        {
            currentCeil += speed * dt;
            if (currentCeil >= maxCeil)
            {
                currentCeil = maxCeil;
                isOpening = false;
                isOpen = true;
            }
        }
    }
};
