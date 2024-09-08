#pragma once

// vendor
#include "glad/glad.h"

// src
#include "Shader.hpp"
#include "ComputeShader.hpp"

struct Grid {
    Grid() = default;
    ~Grid();

    void Init(const int window_width, const int window_height);

    void Update();
    void Draw() const;
    void SetCellColor(const int row, const int col, glm::vec3 color);

    Shader shader{ "fsq" };
    ComputeShader compShader{ "grid_update" };

    GLuint vao;
    GLuint texture_id_curr;
    GLuint texture_id_next;

    int cell_size;
    int nof_cols;
    int nof_rows;
};