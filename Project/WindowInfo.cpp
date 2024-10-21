#include "WindowInfo.h"
#include "Utilities.h"

#include <iostream>
#include <vector>

int WindowInfo::SetupWindow()
{
	std::vector<std::string> contents = LoadFileAsStringVector("config.conf");

	if (!contents.empty())
	{
		std::cout << "READING: `config.conf`\n\n";

		for (int i = 0; i < contents.size(); i++)
		{
			// split string via `=`, figure out what the variable name is via a switch case and then grab it's value and use it!
		}

		std::cout << "\nFINISHED READING: `config.conf`\n\n";
	}

	window = nullptr;

	if (!glfwInit())
	{
		std::cout << "WindowInfo.cpp | GLFW Failed to initalize for an unknown reason.\n";
		return -1;
	}

	window = glfwCreateWindow(width, height, title, nullptr, nullptr);

	if (!window)
	{
		std::cout << "WindowInfo.cpp | Failed to create GLFW window!\n";
		glfwTerminate();
		return -1;
	}

	glfwSetWindowUserPointer(window, (void*)this);
	glfwMakeContextCurrent(window);

	auto* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
	if (framerate == 0) framerate = mode->refreshRate;

	std::cout << "OpenGL Version: " << GL_MAJOR_VERSION << "." << GL_MINOR_VERSION << "\n";

	if (!gladLoadGL())
	{
		std::cout << "BasicApp.cpp | Failed to initalize GLAD!\n";
		glfwDestroyWindow(window);
		glfwTerminate();
		return -1;
	}

	glfwSwapInterval(1);

	glClearColor(bgColour[0], bgColour[1], bgColour[2], 1.0f);
	glEnable(GL_DEPTH_TEST);

	return 0;
};

WindowInfo::WindowInfo(unsigned int x, unsigned int y)
{
	width = x;
	height = y;

	aspectRatio = width / (float)height;

	SetupWindow();
};

WindowInfo::WindowInfo(vec2 Resolution)
{
	width = (unsigned int)Resolution.x;
	height = (unsigned int)Resolution.y;

	aspectRatio = width / (float)height;

	SetupWindow();
};

void WindowInfo::SetWindowColour(float colour[3])
{
	bgColour[0] = colour[0];
	bgColour[1] = colour[1];
	bgColour[2] = colour[2];
};