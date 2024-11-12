#pragma once

#include "Maths.h"
#include "Graphics.h"

#include <string>

struct ConfigParameter
{
public:
	std::string Name;
	std::string Value;
};

struct StartupParameters
{
public:
	const char* Title;
	unsigned int Framerate;
	const char* Resolution;
	const char* WindowMode;
};

class WindowInfo
{
private:
	StartupParameters startups; // TODO: FINISH THIS IMPLEMENTATION!

	unsigned int width = 1280;
	unsigned int height = 720;

	float aspectRatio = 0.0f;

	unsigned int framerate = 0;
	float bgColour[3] = { 0.25f, 0.25f, 0.25f };

protected:
	int SetupWindow();
	void ApplyConfig(std::vector<ConfigParameter> Parameters);

public:
	GLFWwindow* window = nullptr;
	bool ContinueRunning = true;

	WindowInfo() = default;
	~WindowInfo() = default;

	WindowInfo(unsigned int x, unsigned int y);
	WindowInfo(vec2 Resolution);

	const char* GetWindowTitle() { return glfwGetWindowTitle(window); };
	void SetWindowTitle(const char* Title) { glfwSetWindowTitle(window, Title); };

	float GetAspectRatio() const { return aspectRatio; }

	void GetWindowResolution(unsigned int& Width, unsigned int& Height) { Width = width; Height = height; };

	unsigned int GetWindowFramerate() const { return framerate; }
	void SetWindowFramerate(unsigned int Framerate) { framerate = Framerate; }; // Empty until implemented!

	float* GetWindowColour() { return bgColour; };
	void SetWindowColour(float colour[3]);
};