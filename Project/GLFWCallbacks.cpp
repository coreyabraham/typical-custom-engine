#include "GLFWCallbacks.h"

#include "Application.h"
#include "Maths.h"

#include "Key.h"

void ResizeCallback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
	Application* programPointer = (Application*)glfwGetWindowUserPointer(window);	//This is an unsafe coercive cast, but we just have to be careful and live with it.
	if (action == GLFW_PRESS)
	{
		programPointer->OnMouseClick(button);
	}
	else if (action == GLFW_RELEASE)
	{
		//We don't just do 'else' here because 'action' can also be GLFW_REPEAT. Operating systems send this when a button or key is held down, depending on the settings, but we're going to ignore it.
		programPointer->OnMouseRelease(button);
	}
}

void MouseWheelCallback(GLFWwindow* window, double xDelta, double yDelta)
{
	Application* programPointer = (Application*)glfwGetWindowUserPointer(window);
	programPointer->OnMouseScroll(xDelta, yDelta);
}

void KeyPressCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	Application* programPointer = (Application*)glfwGetWindowUserPointer(window);
	if (action == GLFW_PRESS)
	{
		programPointer->OnKeyPress((Key)key);
	}
	else if (action == GLFW_RELEASE)
	{
		programPointer->OnKeyRelease((Key)key);
	}
}