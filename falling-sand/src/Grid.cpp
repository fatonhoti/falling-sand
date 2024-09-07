#include "Grid.hpp"

// std
#include <iostream>
#include <unordered_set>

void Grid::Init(const int window_width, const int window_height)
{
    glGenTextures(1, &this->texture_id_curr);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, this->texture_id_curr);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, window_width, window_height, 0, GL_RGBA, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glBindTexture(GL_TEXTURE_2D, 0);

    glGenTextures(1, &this->texture_id_next);
    glActiveTexture(GL_TEXTURE0 + 1);
    glBindTexture(GL_TEXTURE_2D, this->texture_id_next);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, window_width, window_height, 0, GL_RGBA, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glBindTexture(GL_TEXTURE_2D, 0);

    /*
    for (int row = 0; row < nof_rows; row++) {
        for (int col = 0; col < nof_cols; col++) {
            this->SetCellColor(row, col + 3, { 0.0f, 1.0f, 0.0f });
            break;
        }
        break;
    }
    */

}

void Grid::Update()
{

    this->compShader.Bind();
    this->compShader.SetInteger1(cell_size, "cell_size");
    this->compShader.SetInteger1(nof_cols, "nof_cols");
    this->compShader.SetInteger1(nof_rows, "nof_rows");

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture_id_curr);
    glBindImageTexture(0, texture_id_curr, 0, GL_FALSE, 0, GL_READ_ONLY, GL_RGBA32F);

    glActiveTexture(GL_TEXTURE0 + 1);
    glBindTexture(GL_TEXTURE_2D, texture_id_next);
    glBindImageTexture(1, texture_id_next, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);

    glDispatchCompute(nof_cols, nof_rows, 1);
    glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

    std::swap(texture_id_curr, texture_id_next);
    temp = (temp + 1) % 2;

    glClearTexImage(texture_id_next, 0, GL_RGBA, GL_FLOAT, 0);

}

void Grid::SetCellColor(const int row, const int col, glm::vec3 color)
{

    // Convert to texture coordinates (pixel position)
    const int pixelX = col * this->cell_size;
    const int pixelY = (nof_rows - 1 - row) * this->cell_size;

    std::vector<glm::vec4> block(cell_size * cell_size, glm::vec4(color, 1.0f));
    if (temp == 0) {
        glBindTexture(GL_TEXTURE_2D, this->texture_id_curr);
    }
    else {
        glBindTexture(GL_TEXTURE_2D, this->texture_id_next);
    }
    glTexSubImage2D(GL_TEXTURE_2D, 0, pixelX, pixelY, cell_size, cell_size, GL_RGBA, GL_FLOAT, block.data());
}

void Grid::Draw()
{
    if (!vao) {
        glGenVertexArrays(1, &vao);
    }

    this->shader.Bind();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, this->texture_id_curr);

    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}
