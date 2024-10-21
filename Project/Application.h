#pragma once

#include "AppUI.h"
#include "WindowInfo.h"
#include "ShaderProgram.h"

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

	WindowInfo windowInfo;
	
	ShaderProgram* shader;
	AppUI* ui;

	Camera camera;

	GLuint vertexBufferID = 0;

	LuaInCPP::Controller LuaController;

	std::vector<Light*> lights;
	std::vector<GameObject*> objects;
protected:

	bool IsRunning() const;
	void Update(float deltaTime);
public:
	Application(int argc, char* argv[]);
	~Application();

	void Run();
	void ManageUI(float deltaTime);
};