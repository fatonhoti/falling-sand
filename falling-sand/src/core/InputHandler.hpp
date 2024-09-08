#pragma once

// vendor
#include "GLFW/glfw3.h"
#include "glm.hpp"

class InputHandler
{
public:
	[[nodiscard]] static void UpdateKeyInput(int key, int action);
	[[nodiscard]] static int KeyPressed(int key);
	[[nodiscard]] static int KeyReleased(int key);

	[[nodiscard]] static void UpdateButtonInput(int button, int action);
	[[nodiscard]] static int ButtonPressed(int button);
	[[nodiscard]] static int ButtonReleased(int button);

	[[nodiscard]] static void UpdateLatestCursorPosition(double xpos, double ypos);
	[[nodiscard]] static glm::vec2 GetLatestCursorPosition();

private:
	static bool PressedKeys[348];
	static bool ReleasedKeys[348];

	static bool PressedButtons[8];
	static bool ReleasedButtons[8];

	static double LatestCursorXPOS;
	static double LatestCursorYPOS;
};
