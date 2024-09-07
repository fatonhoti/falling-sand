#pragma once

// vendor
#include "glad/glad.h"

// std
#include <vector>

// src
#include "Shader.hpp"

class InstancedQuadRenderer {
public:
	InstancedQuadRenderer(const int max_nof_cells);
	~InstancedQuadRenderer();

	void DrawQuads(const int nof_quads, const int quad_size);
	void SetProjectionMatrix(const glm::mat4& pm);

	void SetCell(const int idx, const glm::vec3 color);

	void SetPositionBuffer(const float* buffer, const size_t size);

	float* GetMappedColorBuffer();
	void SetColorBuffer(const float* buffer, const size_t size);

private:
	Shader shader{ "instancedQuad" };
	glm::mat4 projectionMatrix;

	GLuint vao;

	GLuint quadVertices_vbo;
	GLuint quadVertices_ebo;

	GLuint instancedQuadPosition_vbo; // static
	GLuint instancedQuadColor_vbo;	  // dynamic

	void* persistentlyMappedColorBuffer;
	void InitRenderData(const int max_nof_cells);
};