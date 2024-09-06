// std
#include <iostream>

// src
#include "Application.hpp"
#include "FallingSand.hpp"

int main()
{

    Application& app = Application::GetInstance();

    if (app.Init(800, 600, "Falling Sand")) {
        std::cout << "[MAIN][ERROR] Application failed to initialize.\n";
        return -1;
    }

    app.AttachAppInstance(std::move(std::make_unique<FallingSand>()));
    app.Start();
    app.Destroy();

    return 0;
}
