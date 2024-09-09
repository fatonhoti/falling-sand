// vendor
#include "gtc/matrix_transform.hpp"

// std
#include <format>
#include <random>
static std::random_device dev;
static std::mt19937 rng(dev());

// src
#include "FallingSand.hpp"
#include "core/InputHandler.hpp"

static auto hsv_to_rgb = [](const float h, const float s, const float v) {
	// assumes varying hue, but s=v=1
	float kr = std::fmodf(5.0f + h * 6.0f, 6.0f);
	float kg = std::fmodf(3.0f + h * 6.0f, 6.0f);
	float kb = std::fmodf(1.0f + h * 6.0f, 6.0f);
	return glm::vec3{
		1 - std::max(0.0f, std::min({kr, 4.0f - kr, 1.0f})),
		1 - std::max(0.0f, std::min({kg, 4.0f - kg, 1.0f})),
		1 - std::max(0.0f, std::min({kb, 4.0f - kb, 1.0f})),
	};
};

static auto next_color = [] {
	static float hue = 0.0f;
	static glm::vec3 color{ 0.0f };
	hue = std::fmodf(hue + 0.01f, 360.0f);
	color = hsv_to_rgb(hue / 100.0f, 1.0f, 1.0f);
	return color;
};

int FallingSand::Init()
{
	int w, h;
	glfwGetFramebufferSize(this->window_handle, &w, &h);

	const int CELL_SIZE = 1;
	this->grid.cell_size = CELL_SIZE;
	this->grid.nof_cols = w / CELL_SIZE;
	this->grid.nof_rows = h / CELL_SIZE;

	if (this->grid.Init(w, h))
		return -1;

	return 0;
}

int FallingSand::Tick(const double dt) {

	if (InputHandler::IsKeyPressed(GLFW_KEY_ESCAPE))
		return -1;

	const double fps = 1.0 / dt;
	const double dt_ms = dt * 1000.0;
	glfwSetWindowTitle(this->window_handle, std::vformat("dt={:.2f} ms, fps={:.0f}", std::make_format_args(dt_ms, fps)).c_str());

	if (InputHandler::IsButtonPressed(GLFW_MOUSE_BUTTON_LEFT)) {
		const glm::vec2 cursorPos = InputHandler::GetCursorPosition();
		const int col = static_cast<int>(std::floor(cursorPos.x / this->grid.cell_size));
		const int row = static_cast<int>(std::floor(cursorPos.y / this->grid.cell_size));

		if (col >= 0 && col < this->grid.nof_cols && row >= 0 && row < this->grid.nof_rows) {
			const glm::vec3 c = next_color();

			this->grid.SetCellColor(row, col, c);
			this->grid.SetCellColor(row + 1, col - 1, c);
			this->grid.SetCellColor(row + 2, col, c);
			this->grid.SetCellColor(row + 1, col + 1, c);
			this->grid.SetCellColor(row + 2, col - 2, c);
			this->grid.SetCellColor(row + 4, col, c);
			this->grid.SetCellColor(row + 2, col - 3, c);
		}
	}

	// Spawn new grains of sand every frame
	/*
	static std::uniform_int_distribution<std::mt19937::result_type> dist(0, (this->grid.nof_cols - 1));
	for (int i = 5; i; --i)
		this->grid.SetCellColor(0, dist(rng), next_color());
	*/

	this->grid.Update();
	this->grid.Draw();

	return 0;

}

void FallingSand::Destroy() {}