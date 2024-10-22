#pragma once

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
protected:
	bool IsRunning() const;
	void Update(float deltaTime);

	WindowInfo windowInfo;
	bool ImGuiWantsMouse;

	LuaInCPP::Controller LuaController;

	std::vector<Light*> lights;
	std::vector<GameObject*> objects;
public:
	Application(int argc, char* argv[]);
	~Application();

	void Run();

	void OnMouseClick(int mouseButton);
	void OnMouseRelease(int mouseButton);
	void OnMouseScroll(bool positive);

	void OnKeyPress(Key key);
	void OnKeyRelease(Key key);
};