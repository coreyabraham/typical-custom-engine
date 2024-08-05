#pragma once

#include "Maths.h"
#include "Graphics.h"

class WindowInfo
{
private:
	unsigned int width = 1280;
	unsigned int height = 720;

	float aspectRatio = 0.0f;

	unsigned int framerate = 0;
	const char* title = "Application";
	float bgColour[3] = { 0.25f, 0.25f, 0.25f };

protected:
	int SetupWindow();

public:
	GLFWwindow* window = nullptr;
	bool ContinueRunning = true;

	WindowInfo() = default;
	~WindowInfo() = default;

	WindowInfo(unsigned int x, unsigned int y);
	WindowInfo(vec2 Resolution);

	const char* GetWindowTitle() { return title; };
	void SetWindowTitle(const char* Title) { title = Title; };

	float GetAspectRatio() const { return aspectRatio; }
	void SetAspectRatio(float AspectRatio) {  } // Empty until implemented!

	unsigned int GetWindowFramerate() const { return framerate; }
	void SetWindowFramerate(unsigned int Framerate) {  }; // Empty until implemented!

	float* GetWindowColour() { return bgColour; };
	void SetWindowColour(float colour[3]);
};