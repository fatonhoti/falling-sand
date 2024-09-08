#include "InputHandler.hpp"

// https://www.glfw.org/docs/latest/group__keys.html
bool InputHandler::PressedKeys[348] = { false };
bool InputHandler::ReleasedKeys[348] = { false };

// https://www.glfw.org/docs/latest/group__buttons.html
bool InputHandler::PressedButtons[8] = { false };
bool InputHandler::ReleasedButtons[8] = { false };

double InputHandler::LatestCursorXPOS = 1.0f;
double InputHandler::LatestCursorYPOS = 1.0f;

void InputHandler::UpdateKeyInput(int key, int action)
{
	switch (action) {
	case GLFW_PRESS:
		PressedKeys[key] = true;
		ReleasedKeys[key] = false;
		break;
	case GLFW_RELEASE:
		ReleasedKeys[key] = true;
		PressedKeys[key] = false;
		break;
	default:
		// uh...
		break;
	};
}

int InputHandler::KeyPressed(int key)
{
	return PressedKeys[key];
}

int InputHandler::KeyReleased(int key)
{
	return ReleasedKeys[key];
}

void InputHandler::UpdateButtonInput(int button, int action)
{
	switch (action) {
	case GLFW_PRESS:
		PressedButtons[button] = true;
		ReleasedButtons[button] = false;
		break;
	case GLFW_RELEASE:
		ReleasedButtons[button] = true;
		PressedButtons[button] = false;
		break;
	default:
		// uh...
		break;
	};
}

int InputHandler::ButtonPressed(int button)
{
	return PressedButtons[button];
}

int InputHandler::ButtonReleased(int button)
{
	return ReleasedButtons[button];
}

void InputHandler::UpdateLatestCursorPosition(double xpos, double ypos)
{
	LatestCursorXPOS = xpos;
	LatestCursorYPOS = ypos;
}

glm::vec2 InputHandler::GetLatestCursorPosition()
{
	return { LatestCursorXPOS, LatestCursorYPOS };
}
