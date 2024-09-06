#pragma once

// std
#include <memory>

// src
#include "AppInstance.hpp"
#include "Grid.hpp"
#include "InstancedQuadRenderer.hpp"

class FallingSand : public AppInstance {
public:
    FallingSand() = default;

    int Init() override;
    int Tick() override;
    void Destroy() override;

private:
    Grid grid;
    std::unique_ptr<InstancedQuadRenderer> quadRenderer{ nullptr };
};