#include "GLFWCallbacks.h"

void ResizeCallback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}