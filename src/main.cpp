#include "core/Application.hpp"
#include <iostream>

int main()
{
    Application app;
    if (!app.init())
    {
        std::cerr << "Failed to initialize ABYSS application.\n";
        return 1;
    }

    app.run();
    return 0;
}

