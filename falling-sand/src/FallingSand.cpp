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
	const int NOF_FLOATS_PER_COLOR = 3;
	std::vector<float> data(MAX_NOF_CELLS * NOF_FLOATS_PER_POSITION * NOF_FLOATS_PER_COLOR, 0.0f);
	size_t offset = 0;
	for (int row = 0; row < this->grid.nof_rows; row++) {
		for (int col = 0; col < this->grid.nof_cols; col++) {
			data[offset    ] = col * this->grid.cell_size;
			data[offset + 1] = row * this->grid.cell_size;
			data[offset + 2] = 0.0f; // r
			data[offset + 3] = 0.0f; // g
			data[offset + 4] = 0.0f; // b
			offset += 5;
		}
	}
	this->quadRenderer->SetBuffer(data.data(), data.size() * sizeof(float));

	return 0;
}

int FallingSand::Tick(const double dt) {
	static int NOF_QUADS_TO_DRAW = this->grid.nof_cols * this->grid.nof_rows;

	const double fps = 1.0 / dt;
	glfwSetWindowTitle(this->window_handle, std::format("dt={:.2f} ms, fps={:.0f}, nCells={}", dt * 1000.0, fps, this->grid.cells.size()).c_str());

	if (InputHandler::ButtonPressed(GLFW_MOUSE_BUTTON_LEFT)) {
		const glm::vec2 cursorPos = InputHandler::GetLatestCursorPosition();

		const int col = static_cast<int>(std::floor(cursorPos.x / this->grid.cell_size));
		const int row = static_cast<int>(std::floor(cursorPos.y / this->grid.cell_size));

		static float hue = 1.0f;

		hue = std::fmodf(hue + 0.20f, 361.0f);

		static glm::vec3 color;
		color = hsv_to_rgb(hue / 360.0f, 1.0f, 1.0f);

		// each vertex has [px py r g b], so 5 floats
		int idx = row * (5 * this->grid.nof_cols) + (5 * col);
		int idx2 = (row + 1) * (5 * this->grid.nof_cols) + (5 * (col - 1));
		int idx3 = (row + 2) * (5 * this->grid.nof_cols) + (5 * col);
		int idx4 = (row + 1) * (5 * this->grid.nof_cols) + (5 * (col + 1));

		this->grid.cells.insert(idx);
		this->grid.cells.insert(idx2);
		this->grid.cells.insert(idx3);
		this->grid.cells.insert(idx4);
		quadRenderer->SetCell(idx, color);
		quadRenderer->SetCell(idx2, color);
		quadRenderer->SetCell(idx3, color);
		quadRenderer->SetCell(idx4, color);

	}

	this->grid.Update(this->quadRenderer->GetMappedBuffer());
	this->quadRenderer->DrawQuads(NOF_QUADS_TO_DRAW, this->grid.cell_size);

	return 0;

}

void FallingSand::Destroy() {}