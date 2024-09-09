#pragma once

// vendor
#include "GLFW/glfw3.h"
#include "glm.hpp"

class InputHandler
{
public:
    // Update key input state
    static void UpdateKeyInput(int key, int action);

    // Check if a key is currently pressed or released
    [[nodiscard]] static bool IsKeyPressed(int key);
    [[nodiscard]] static bool IsKeyReleased(int key);

    // Update button input state
    static void UpdateButtonInput(int button, int action);

    // Check if a mouse button is currently pressed or released
    [[nodiscard]] static bool IsButtonPressed(int button);
    [[nodiscard]] static bool IsButtonReleased(int button);

    // Update the cursor position
    static void UpdateCursorPosition(double xpos, double ypos);

    // Get the latest cursor position
    [[nodiscard]] static glm::vec2 GetCursorPosition();

private:
    static bool PressedKeys[GLFW_KEY_LAST + 1];
    static bool ReleasedKeys[GLFW_KEY_LAST + 1];

    static bool PressedButtons[GLFW_MOUSE_BUTTON_LAST + 1];
    static bool ReleasedButtons[GLFW_MOUSE_BUTTON_LAST + 1];

    static double CursorXPos;
    static double CursorYPos;
};