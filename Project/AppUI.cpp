#include <iostream>

#include "AppUI.h"
#include "Application.h"

#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

AppUI::~AppUI()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

void AppUI::Initalize(GLFWwindow* window, bool installCallbacks, float pixelDensityScale = -1.0f)
{
	ImGui::CreateContext();

	ImGui_ImplGlfw_InitForOpenGL(window, installCallbacks);
	ImGui_ImplOpenGL3_Init();

	if (pixelDensityScale != -1.0f) ImGui::GetIO().FontGlobalScale = pixelDensityScale;
}

void AppUI::Update(Application* app, float deltaTime)
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	app->ManageUI(deltaTime);
	ImGui::Render();
}

void AppUI::Render()
{
	ImDrawData* data = ImGui::GetDrawData();
	if (!data) return;
	ImGui_ImplOpenGL3_RenderDrawData(data);
}