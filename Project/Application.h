#pragma once

constexpr auto TCE_VERSION = 0.01;
constexpr auto TCE_AUTHOR = "Corey Abraham";

#include <map>

#include "WindowInfo.h"
#include "ShaderProgram.h"

#include "Key.h"

#include "Shapes.h"
#include "Utilities.h"
#include "Graphics.h"

#include "Camera.h"
#include "Light.h"

#include "Mesh.h"
#include "Texture.h"

#include "LuaInCPP.h"

class Application
{
private:
	vec2 cursorPos = { 0.0f, 0.0f };
	vec2 Resolution = { 1280, 720 };
	
	ShaderProgram* shader;
	Camera camera;

	GLuint vertexBufferID = 0;

	float deltaTime = 0.0f;
protected:
	bool isSceneLoaded = false;

	// <TODO> TEMPORARY VALUE, PLEASE REPLACE WITH POINTER TO LOADED SCENE CLASS!
	int currentlyLoadedScene = 0;

	void Run();
	
	void Update();
	void Render();
	void Debug();

	WindowInfo windowInfo;
	bool ImGuiWantsMouse;

	LuaInCPP::Controller LuaController;

	std::vector<Light*> lights;
	std::vector<GameObject*> objects;

	std::map<int, bool> keyCache;
public:
	Application() = default;
	Application(int argc, char* argv[]);
	~Application();

	WindowInfo* GetWindowInfo() { return &windowInfo; }

	bool IsRunning() const;

	void OnMouseClick(int mouseButton);
	void OnMouseRelease(int mouseButton);
	void OnMouseScroll(double xDelta, double yDelta);

	void OnKeyPress(Key key);
	void OnKeyRelease(Key key);
};