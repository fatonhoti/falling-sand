// vendor
#include "gtc/matrix_transform.hpp"

// std
#include <iostream>
#include <format>
#include <random>
static std::random_device dev;
static std::mt19937 rng(dev());

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

	this->grid.Init(w, h);

	return 0;
}

int FallingSand::Tick(const double dt) {

	const double fps = 1.0 / dt;
	const double dt_ms = dt * 1000.0;
	glfwSetWindowTitle(this->window_handle, std::vformat("dt={:.2f} ms, fps={:.0f}", std::make_format_args(dt_ms, fps)).c_str());

	static float hue = 1.0f;
	static glm::vec3 color{ 0.0f };

	static bool done_once = false;

	if (!done_once && InputHandler::ButtonPressed(GLFW_MOUSE_BUTTON_LEFT)) {
		//done_once = true;

		hue = std::fmodf(hue + 0.005f, 361.0f);
		color = hsv_to_rgb(hue / 360.0f, 1.0f, 1.0f);

		const glm::vec2 cursorPos = InputHandler::GetLatestCursorPosition();

		const int col = static_cast<int>(std::floor(cursorPos.x / this->grid.cell_size));
		const int row = static_cast<int>(std::floor(cursorPos.y / this->grid.cell_size));

		// Ensure the coordinates are within the bounds of the grid
		if (col >= 0 && col < this->grid.nof_cols && row >= 0 && row < this->grid.nof_rows) {
			this->grid.SetCellColor(row, col, color);
		}
	}

	static std::uniform_int_distribution<std::mt19937::result_type> dist6(0, this->grid.nof_cols - 1); // distribution in range [1, 6]

	for (int i = 0; i < 5; i++) {
		hue = std::fmodf(hue + 0.005f, 361.0f);
		color = hsv_to_rgb(hue / 360.0f, 1.0f, 1.0f);
		this->grid.SetCellColor(1, dist6(rng), color);
	}

	this->grid.Update();
	this->grid.Draw();

	return 0;

}

void FallingSand::Destroy() {}