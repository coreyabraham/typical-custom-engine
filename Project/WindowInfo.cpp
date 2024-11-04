#include "WindowInfo.h"
#include "Utilities.h"

#include <iostream>
#include <sstream>
#include <vector>

void WindowInfo::ApplyConfig(std::vector<ConfigParameter> Parameters)
{
	for (ConfigParameter param : Parameters)
	{
		// TODO: IMPROVE THIS ALGORITHM AND MAKE IT MODULATED (Similar to Lua method listed below)!
		/*
			local methods = {
				[1] = function()
					print("Hello World!")
				end,
	
				[2] = function()
					print("World Hello!")
				end
			}

			local target = methods[1]

			if not target or typeof(target) ~= "function" then
				return
			end

			target()
		*/

		if (param.Name == "WindowTitle")
		{
			std::cout << "Change Window Title to: " << param.Value << "!\n";
			//WindowTitle = param.Value.c_str();
		}

		else if (param.Name == "TargetFrameRate")
		{
			std::cout << "Change Target Frame Rate to: " << param.Value << "!\n";
		}

		else if (param.Name == "Resolution")
		{
			std::cout << "Change Game / Window Resolution to: " << param.Value << "!\n";
		}

		else if (param.Name == "WindowMode")
		{
			std::cout << "Change Window Mode to: " << param.Value << "!\n";
		}
	}
}

int WindowInfo::SetupWindow()
{
	std::vector<std::string> contents = LoadFileAsStringVector("config.conf");

	if (!contents.empty())
	{
		std::vector<ConfigParameter> parameters;

		std::cout << "READING: `config.conf`\n\n";

		for (int i = 0; i < contents.size(); i++)
		{
			// TODO: split string via `=`, figure out what the variable name is via a switch case and then grab it's value and use it!

			// original line
			//std::cout << contents[i] << "\n";

			// splitting process!
			std::stringstream stream(contents[i]);
			std::string temporary;

			char separator = '=';

			ConfigParameter config;
			unsigned int count = 0;

			// This may not be an optimized way of doing this... consider changing it in the future!
			while (std::getline(stream, temporary, separator))
			{
				count++;

				if (count > 2) break;

				if (config.Name == "")
				{
					config.Name = temporary;
					continue;
				}

				if (config.Value == "")
				{
					config.Value = temporary;
					continue;
				}
			}

			parameters.push_back(config);
		}

		std::cout << "\nFINISHED READING: `config.conf`\n\n";
		ApplyConfig(parameters);
	}

	if (!glfwInit())
	{
		std::cout << "WindowInfo.cpp | GLFW Failed to initalize for an unknown reason.\n";
		return -1;
	}

	window = glfwCreateWindow(width, height, "WindowTitle", nullptr, nullptr);

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