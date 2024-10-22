#pragma once

#include "Graphics.h"

void ResizeCallback(GLFWwindow* window, int width, int height);
void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
void MouseWheelCallback(GLFWwindow* window, double xDelta, double yDelta);
void KeyPressCallback(GLFWwindow* window, int key, int scancode, int action, int mods);