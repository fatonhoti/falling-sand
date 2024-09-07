#include "Grid.hpp"

std::random_device rand_device;
std::mt19937 generator(rand_device());
std::uniform_int_distribution<int> distr01(0, 1);

void Grid::Update(float* buffer)
{

    std::unordered_set<int> future;
    future.reserve(cells.size());

    // Only iterates over 'active' (colored) cells.
    for (const auto& cell : cells) {

        // each vertex has [px py r g b], so 5 floats
        const int row = cell / (5 * nof_cols);
        const int col = (cell % (5 * nof_cols)) / 5;

        // Cell already on the floor.
        if (row == nof_rows - 1) {
            future.insert(cell);
            continue;
        }

        // Try to go down.
        int idxSouth = (row + 1) * (5 * nof_cols) + (5 * col);
        if (!cells.contains(idxSouth)) {
            future.insert(idxSouth);

            buffer[idxSouth + 2] = buffer[cell + 2];
            buffer[idxSouth + 3] = buffer[cell + 3];
            buffer[idxSouth + 4] = buffer[cell + 4];

            buffer[cell + 2] = 0.0f;
            buffer[cell + 3] = 0.0f;
            buffer[cell + 4] = 0.0f;


            continue;
        }

        // Couldn't go down, let's try to go down and left/right then.
        const int dir = 2 * distr01(generator) - 1;

        // Can't move left or right either => Stay put.
        if (col + dir < 0 || col + dir >= nof_cols) {
            future.insert(cell);
            continue;
        }

        int idxLateral = (row + 1) * (5 * nof_cols) + (5 * (col + dir));
        if (!cells.contains(idxLateral)) {
            future.insert(idxLateral);

            buffer[idxLateral + 2] = buffer[cell + 2];
            buffer[idxLateral + 3] = buffer[cell + 3];
            buffer[idxLateral + 4] = buffer[cell + 4];

            buffer[cell + 2] = 0.0f;
            buffer[cell + 3] = 0.0f;
            buffer[cell + 4] = 0.0f;

            continue;
        }

        // No movement.
        future.insert(cell);

    }

    cells = std::move(future);
}
