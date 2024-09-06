#pragma once

// std
#include <unordered_set>
#include <random>

struct Grid {
    int cell_size;
    int nof_cols;
    int nof_rows;
    std::unordered_set<int> cells;
    void Update(float* buffer);
};