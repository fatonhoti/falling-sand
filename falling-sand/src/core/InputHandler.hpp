#pragma once

// vendor
#include "GLFW/glfw3.h"
#include "glm.hpp"

class InputHandler
{
public:
    static void UpdateKeyInput(int key, int action);

    [[nodiscard]] static bool IsKeyPressed(int key);
    [[nodiscard]] static bool IsKeyReleased(int key);

    static void UpdateButtonInput(int button, int action);

    [[nodiscard]] static bool IsButtonPressed(int button);
    [[nodiscard]] static bool IsButtonReleased(int button);

    static void UpdateCursorPosition(double xpos, double ypos);

    [[nodiscard]] static glm::vec2 GetCursorPosition();

private:
    static bool PressedKeys[GLFW_KEY_LAST + 1];
    static bool ReleasedKeys[GLFW_KEY_LAST + 1];

    static bool PressedButtons[GLFW_MOUSE_BUTTON_LAST + 1];
    static bool ReleasedButtons[GLFW_MOUSE_BUTTON_LAST + 1];

    static double CursorXPos;
    static double CursorYPos;
};