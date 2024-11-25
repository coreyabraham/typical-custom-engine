#pragma once

#include "Maths.h"
#include "Graphics.h"

#include <string>

class WindowInfo
{
private:
	struct ConfigParameter
	{
	public:
		std::string Name;
		std::string Value;
	};

	float aspectRatio;

	unsigned int width = 1280;
	unsigned int height = 720;
	unsigned int framerate = 0;

	std::string title = "GLFW Window";

	// <TODO> Please assign this to the newer `Scene` class being made soon!
	float bgColour[3] = { 0.25f, 0.25f, 0.25f };

	bool calledWindowSetup = false;
protected:
	void SetupWindow();
	void SetupConfig();
	void ApplyConfig(std::vector<ConfigParameter> Parameters);

public:
	GLFWwindow* window;
	bool ContinueRunning = true;

	WindowInfo() = default;
	~WindowInfo();

	WindowInfo(unsigned int x, unsigned int y);
	WindowInfo(vec2 Resolution);

	const char* GetWindowTitle() const { return glfwGetWindowTitle(window); };
	void SetWindowTitle(const char* Title) const { glfwSetWindowTitle(window, Title); };

	float GetAspectRatio() const { return aspectRatio; }
	void SetAspectRatio() { aspectRatio = width / (float)height; }

	std::pair<unsigned int, unsigned int> GetWindowResolution() const { return std::make_pair(width, height); };
	void SetWindowResolution(unsigned int Width, unsigned int Height) { width = Width, height = Height; };
	void SetWindowResolution(vec2 Resolution) { width = (unsigned int)Resolution.x; height = (unsigned int)Resolution.y; };

	unsigned int GetWindowFramerate() const { return framerate; }
	void SetWindowFramerate(unsigned int Framerate) { framerate = Framerate; }; // Empty until implemented!

	float* GetWindowColour() { return bgColour; };
	void SetWindowColour(float colour[3]);
};