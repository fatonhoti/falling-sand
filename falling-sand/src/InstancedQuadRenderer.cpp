#include "InstancedQuadRenderer.hpp"

InstancedQuadRenderer::InstancedQuadRenderer(const int max_nof_cells) {
    this->InitRenderData(max_nof_cells);
}

InstancedQuadRenderer::~InstancedQuadRenderer() {
    if (mappedBuffer) {
        glUnmapBuffer(this->instancedQuadPosition_vbo);
    }
    glDeleteBuffers(1, &this->quadVertices_vbo);
    glDeleteBuffers(1, &this->quadVertices_ebo);
    glDeleteBuffers(1, &this->instancedQuadPosition_vbo);
    glDeleteVertexArrays(1, &vao);
}

void InstancedQuadRenderer::DrawQuads(const int nof_quads, const int quad_size)
{

    this->shader.Bind();
    this->shader.SetMat4x4(this->projectionMatrix, "u_projMat");
    this->shader.SetInteger1(quad_size, "u_QuadSize");

    glBindVertexArray(this->vao);
    glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, nof_quads);
}

void InstancedQuadRenderer::SetProjectionMatrix(const glm::mat4& pm)
{
    this->projectionMatrix = pm;
}

void InstancedQuadRenderer::SetCell(const int idx, const glm::vec3 color)
{
    float* buffer = static_cast<float*>(mappedBuffer);
    buffer[idx + 2] = color.r;
    buffer[idx + 3] = color.g;
    buffer[idx + 4] = color.b;
}

void InstancedQuadRenderer::ResetCell(const int idx)
{
    float* buffer = static_cast<float*>(mappedBuffer);
    buffer[idx + 2] = 0.0f;
    buffer[idx + 3] = 0.0f;
    buffer[idx + 4] = 0.0f;
}

float* InstancedQuadRenderer::GetMappedBuffer()
{
    return static_cast<float*>(mappedBuffer);
}

void InstancedQuadRenderer::SetBuffer(const float* buffer, const size_t size)
{
    memcpy(mappedBuffer, buffer, size);
}

void InstancedQuadRenderer::InitRenderData(const int max_nof_cells) {

    static constexpr float vertices[] = {
        // Triangle 1 (left)
        0.0f, 1.0f, // TL
        1.0f, 0.0f, // BR
        0.0f, 0.0f, // BL
        1.0f, 1.0f, // TR
    };

    static constexpr unsigned int indices[] = {
        0, 1, 2,
        0, 1, 3,
    };

    glGenVertexArrays(1, &this->vao);
    glBindVertexArray(this->vao);

    glGenBuffers(1, &this->quadVertices_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, this->quadVertices_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);

    glGenBuffers(1, &this->quadVertices_ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->quadVertices_ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Position and color attribute (per instance)
    glGenBuffers(1, &this->instancedQuadPosition_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, this->instancedQuadPosition_vbo);

    const int NOF_FLOATS_PER_POSITION = 2;
    const int NOF_FLOATS_PER_COLOR = 3;
    const int BYTES_PER_CELL = NOF_FLOATS_PER_POSITION * NOF_FLOATS_PER_COLOR * sizeof(float);
    const int BYTES_ALL_CELLS = max_nof_cells * BYTES_PER_CELL;

    const auto flags = GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT | GL_MAP_WRITE_BIT;
    glBufferStorage(GL_ARRAY_BUFFER, BYTES_ALL_CELLS, NULL, flags);
    mappedBuffer = glMapBufferRange(GL_ARRAY_BUFFER, 0, BYTES_ALL_CELLS, flags);

    // Position (technically useless, but kept here in case I wanna rework into particle system)
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, NOF_FLOATS_PER_POSITION, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glVertexAttribDivisor(1, 1); // Update once per instance

    // Color
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, NOF_FLOATS_PER_COLOR, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(NOF_FLOATS_PER_POSITION * sizeof(float)));
    glVertexAttribDivisor(2, 1); // Update once per instance

}
