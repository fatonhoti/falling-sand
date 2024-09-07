// vendor
#include "gtc/matrix_transform.hpp"

// std
#include <format>

// src
#include "FallingSand.hpp"
#include "InputHandler.hpp"

static auto hsv_to_rgb = [](const float h, const float s, const float v) {
	float kr = std::fmodf(5.0f + h * 6.0f, 6.0f);
	float kg = std::fmodf(3.0f + h * 6.0f, 6.0f);
	float kb = std::fmodf(1.0f + h * 6.0f, 6.0f);
	return glm::vec3{
		1 - std::max(0.0f, std::min({kr, 4.0f - kr, 1.0f})),
		1 - std::max(0.0f, std::min({kg, 4.0f - kg, 1.0f})),
		1 - std::max(0.0f, std::min({kb, 4.0f - kb, 1.0f})),
	};
	};

int FallingSand::Init()
{
	int w, h;
	glfwGetFramebufferSize(this->window_handle, &w, &h);

	const int CELL_SIZE = 1;
	this->grid.cell_size = CELL_SIZE;
	this->grid.nof_cols = w / CELL_SIZE;
	this->grid.nof_rows = h / CELL_SIZE;

	const int MAX_NOF_CELLS = this->grid.nof_cols * this->grid.nof_rows;
	const glm::mat4 projMat = glm::ortho(0.0f, static_cast<float>(w), static_cast<float>(h), 0.0f, -1.0f, 1.0f);

	this->quadRenderer = std::make_unique<InstancedQuadRenderer>(MAX_NOF_CELLS);
	this->quadRenderer->SetProjectionMatrix(projMat);

	// Initialize persistently mapped buffer
	const int NOF_FLOATS_PER_POSITION = 2;
	std::vector<float> positions(MAX_NOF_CELLS * NOF_FLOATS_PER_POSITION, 0.0f);

	const int NOF_FLOATS_PER_COLOR = 3;
	std::vector<float> colors(MAX_NOF_CELLS * NOF_FLOATS_PER_COLOR, 0.0f);

	size_t offset_pos = 0;
	size_t offset_col = 0;
	for (int row = 0; row < this->grid.nof_rows; row++) {
		for (int col = 0; col < this->grid.nof_cols; col++) {

			positions[offset_pos    ] = col * this->grid.cell_size;
			positions[offset_pos + 1] = row * this->grid.cell_size;
			offset_pos += 2;

			colors[offset_col + 2] = 0.0f; // r
			colors[offset_col + 3] = 0.0f; // g
			colors[offset_col + 4] = 0.0f; // b
			offset_col += 3;

		}
	}

	this->quadRenderer->SetPositionBuffer(positions.data(), positions.size() * sizeof(float));
	this->quadRenderer->SetColorBuffer(colors.data(), colors.size() * sizeof(float));

	return 0;
}

int FallingSand::Tick(const double dt) {
	static int NOF_QUADS_TO_DRAW = this->grid.nof_cols * this->grid.nof_rows;

	const double fps = 1.0 / dt;
	glfwSetWindowTitle(this->window_handle, std::format("dt={:.2f} ms, fps={:.0f}, nCells={}", dt * 1000.0, fps, this->grid.cells.size()).c_str());

	static float hue = 1.0f;
	static glm::vec3 color{ 0.0f };

	if (InputHandler::ButtonPressed(GLFW_MOUSE_BUTTON_LEFT)) {
		const glm::vec2 cursorPos = InputHandler::GetLatestCursorPosition();

		const int col = static_cast<int>(std::floor(cursorPos.x / this->grid.cell_size));
		const int row = static_cast<int>(std::floor(cursorPos.y / this->grid.cell_size));

		hue = std::fmodf(hue + 0.20f, 361.0f);
		color = hsv_to_rgb(hue / 360.0f, 1.0f, 1.0f);

		// each vertex has [px py r g b], so 5 floats
		int idx = row * (3 * this->grid.nof_cols) + (3 * col);
		int idx2 = (row + 1) * (3 * this->grid.nof_cols) + (3 * (col - 1));
		int idx3 = (row + 2) * (3 * this->grid.nof_cols) + (3 * col);
		int idx4 = (row + 1) * (3 * this->grid.nof_cols) + (3 * (col + 1));

		if (!this->grid.cells.contains(idx)) {
			this->grid.cells.insert(idx);
			quadRenderer->SetCell(idx, color);
		}
		if (!this->grid.cells.contains(idx2)) {
			this->grid.cells.insert(idx2);
			quadRenderer->SetCell(idx2, color);
		}
		if (!this->grid.cells.contains(idx3)) {
			this->grid.cells.insert(idx3);
			quadRenderer->SetCell(idx3, color);
		}
		if (!this->grid.cells.contains(idx4)) {
			this->grid.cells.insert(idx4);
			quadRenderer->SetCell(idx4, color);
		}

	}

	for (int col = 0; col < this->grid.nof_cols; col += 1) {
		hue = std::fmodf(hue + 0.01f, 361.0f);
		color = hsv_to_rgb(hue / 360.0f, 1.0f, 1.0f);
		int idx = 1 * (3 * this->grid.nof_cols) + (3 * col);
		if (!this->grid.cells.contains(idx)) {
			this->grid.cells.insert(idx);
			quadRenderer->SetCell(idx, color);
		}
	}

	this->grid.Update(this->quadRenderer->GetMappedColorBuffer());
	this->quadRenderer->DrawQuads(NOF_QUADS_TO_DRAW, this->grid.cell_size);

	return 0;

}

void FallingSand::Destroy() {}