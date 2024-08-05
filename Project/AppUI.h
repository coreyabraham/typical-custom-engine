#pragma once

#include "imgui.h"
#include "Graphics.h"

class Application;

class AppUI
{
public:
	AppUI() = default;
	~AppUI();

	void Initalize(GLFWwindow* window, bool installCallbacks, float pixelDensityScale);

	void Update(Application* app, float deltaTime);
	void Render();
};