// std
#include <iostream>

// src
#include "core/Application.hpp"
#include "FallingSand.hpp"

int main()
{

    Application& app = Application::GetInstance();

    if (app.Init(1200, 800, "Falling Sand")) {
        std::cout << "[MAIN][ERROR] Application failed to initialize.\n";
        return -1;
    }

    if (app.AttachAppInstance(std::move(std::make_unique<FallingSand>())) == 0) {
        app.Start();
    }

    app.Destroy();

    return 0;
}
