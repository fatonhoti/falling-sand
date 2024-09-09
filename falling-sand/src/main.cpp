// std
#include <iostream>

// src
#include "core/Application.hpp"
#include "FallingSand.hpp"

int main()
{

    Application& app = Application::GetInstance();

    if (app.Init(1600, 900, "Falling Sand")) {
        std::cout << "[Main][ERROR] Application failed to initialize.\n";
        return -1;
    }

    if (app.AttachAppInstance(std::move(std::make_unique<FallingSand>())) == 0) {
        app.Start();
    }

    app.Destroy();

    return 0;
}
