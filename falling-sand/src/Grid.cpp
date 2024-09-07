#include "Grid.hpp"

static std::random_device rand_device;
static std::mt19937 generator(rand_device());
static std::uniform_int_distribution<int> distr01(0, 1);

void Grid::Update(float* color_buffer)
{

    static int NOF_FLOATS_PER_COLOR = 3;
    std::unordered_set<int> future;
    future.reserve(cells.size());

    // Only iterates over 'active' (colored) cells.
    for (const auto& cell : cells) {

        const int row = cell / (NOF_FLOATS_PER_COLOR * nof_cols);
        const int col = (cell % (NOF_FLOATS_PER_COLOR * nof_cols)) / NOF_FLOATS_PER_COLOR;

        // Cell already on the floor.
        if (row == nof_rows - 1) {
            future.insert(cell);
            continue;
        }

        // Try to go down.
        int idxSouth = (row + 1) * (NOF_FLOATS_PER_COLOR * nof_cols) + (NOF_FLOATS_PER_COLOR * col);
        if (!cells.contains(idxSouth)) {
            future.insert(idxSouth);

            color_buffer[idxSouth    ] = color_buffer[cell    ];
            color_buffer[idxSouth + 1] = color_buffer[cell + 1];
            color_buffer[idxSouth + 2] = color_buffer[cell + 2];

            color_buffer[cell    ] = 0.0f;
            color_buffer[cell + 1] = 0.0f;
            color_buffer[cell + 2] = 0.0f;

            continue;
        }

        // Couldn't go down, let's try to go down and left/right then.
        const int dir = 2 * distr01(generator) - 1;

        // Can't move left or right either => Stay put.
        if (col + dir < 0 || col + dir >= nof_cols) {
            future.insert(cell);
            continue;
        }

        int idxLateral = (row + 1) * (NOF_FLOATS_PER_COLOR * nof_cols) + (NOF_FLOATS_PER_COLOR * (col + dir));
        if (!cells.contains(idxLateral)) {
            future.insert(idxLateral);

            color_buffer[idxLateral    ] = color_buffer[cell    ];
            color_buffer[idxLateral + 1] = color_buffer[cell + 1];
            color_buffer[idxLateral + 2] = color_buffer[cell + 2];

            color_buffer[cell    ] = 0.0f;
            color_buffer[cell + 1] = 0.0f;
            color_buffer[cell + 2] = 0.0f;

            continue;
        }

        // No movement.
        future.insert(cell);

    }

    cells = std::move(future);
}
