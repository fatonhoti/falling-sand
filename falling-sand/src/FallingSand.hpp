#pragma once

// std
#include <memory>

// src
#include "core/AppInstance.hpp"
#include "Grid.hpp"

class FallingSand : public AppInstance {
public:
    FallingSand() = default;

    int Init() override;
    int Tick(const double dt) override;
    void Destroy() override;

private:
    Grid grid;
};