#pragma once

#include "imgui.h"
#include "Graphics.h"

#include "Application.h"

class AppUI : public Application
{
public:
	AppUI() = default;
	~AppUI() = default;

	void OpenUpdate(float deltaTime);
};