#pragma once

// vendor
#include "glad/glad.h"

// src
#include "Shader.hpp"
#include "ComputeShader.hpp"

struct Grid {
    void Init(const int window_width, const int window_height);
    void Update();
    void SetCellColor(const int row, const int col, glm::vec3 color);
    void Draw();
    Shader shader{ "fsq" };
    ComputeShader compShader{ "grid_update" };
    GLuint texture_id_curr;
    GLuint texture_id_next;
    GLuint vao;
    int cell_size;
    int nof_cols;
    int nof_rows;

    int temp = 0;
};