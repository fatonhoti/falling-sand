#include "InputHandler.hpp"

bool InputHandler::PressedKeys[GLFW_KEY_LAST + 1] = { false };
bool InputHandler::ReleasedKeys[GLFW_KEY_LAST + 1] = { false };

bool InputHandler::PressedButtons[GLFW_MOUSE_BUTTON_LAST + 1] = { false };
bool InputHandler::ReleasedButtons[GLFW_MOUSE_BUTTON_LAST + 1] = { false };

double InputHandler::CursorXPos = 0.0;
double InputHandler::CursorYPos = 0.0;

void InputHandler::UpdateKeyInput(int key, int action)
{
    if (key >= 0 && key <= GLFW_KEY_LAST) {
        PressedKeys[key] = (action == GLFW_PRESS);
        ReleasedKeys[key] = (action == GLFW_RELEASE);
    }
}

bool InputHandler::IsKeyPressed(int key)
{
    return key >= 0 && key <= GLFW_KEY_LAST && PressedKeys[key];
}

bool InputHandler::IsKeyReleased(int key)
{
    return key >= 0 && key <= GLFW_KEY_LAST && ReleasedKeys[key];
}

void InputHandler::UpdateButtonInput(int button, int action)
{
    if (button >= 0 && button <= GLFW_MOUSE_BUTTON_LAST) {
        PressedButtons[button] = (action == GLFW_PRESS);
        ReleasedButtons[button] = (action == GLFW_RELEASE);
    }
}

bool InputHandler::IsButtonPressed(int button)
{
    return button >= 0 && button <= GLFW_MOUSE_BUTTON_LAST && PressedButtons[button];
}

bool InputHandler::IsButtonReleased(int button)
{
    return button >= 0 && button <= GLFW_MOUSE_BUTTON_LAST && ReleasedButtons[button];
}

void InputHandler::UpdateCursorPosition(double xpos, double ypos)
{
    CursorXPos = xpos;
    CursorYPos = ypos;
}

glm::vec2 InputHandler::GetCursorPosition()
{
    return { CursorXPos, CursorYPos };
}