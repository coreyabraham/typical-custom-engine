#pragma once

#include "WindowInfo.h"
#include "Angelscripting.h"
#include "ShaderProgram.h"
#include "AppUI.h"

class Application
{
private:
	vec2 cursorPos = { 0.0f, 0.0f };
	vec2 Resolution = { 1280, 720 };

	WindowInfo windowInfo;
	ShaderProgram* shader;
	AppUI* ui;

	asIScriptEngine* scriptEngine = nullptr;

	GLuint vertexBufferID = 0;
	std::vector<float> someFloats
	{
		0, 0, 0,    0, 0, 1,
		0, 1, 0,    0, 0, 1,
		1, 0, 0,    0, 0, 1,
		0, 1, 0,    0, 0, 1,
		1, 0, 0,    0, 0, 1,
		1, 1, 0,    0, 0, 1,

		0, 0, 1,    1, 1, 0,
		0, 1, 1,    1, 1, 0,
		1, 0, 1,    1, 1, 0,
		0, 1, 1,    1, 1, 0,
		1, 0, 1,    1, 1, 0,
		1, 1, 1,    1, 1, 0,


		0, 0, 0,    0, 1, 0,
		0, 0, 1,    0, 1, 0,
		1, 0, 0,    0, 1, 0,
		0, 0, 1,    0, 1, 0,
		1, 0, 0,    0, 1, 0,
		1, 0, 1,    0, 1, 0,

		0, 1, 0,    1, 0, 1,
		0, 1, 1,    1, 0, 1,
		1, 1, 0,    1, 0, 1,
		0, 1, 1,    1, 0, 1,
		1, 1, 0,    1, 0, 1,
		1, 1, 1,    1, 0, 1,


		0, 0, 0,    1, 0, 0,
		0, 0, 1,    1, 0, 0,
		0, 1, 0,    1, 0, 0,
		0, 0, 1,    1, 0, 0,
		0, 1, 0,    1, 0, 0,
		0, 1, 1,    1, 0, 0,

		1, 0, 0,    0, 1, 1,
		1, 0, 1,    0, 1, 1,
		1, 1, 0,    0, 1, 1,
		1, 0, 1,    0, 1, 1,
		1, 1, 0,    0, 1, 1,
		1, 1, 1,    0, 1, 1,
	};

protected:
	bool IsRunning() const;
	void Update(float deltaTime);
public:
	Application(int argc, char* argv[]);
	~Application();

	void Run();
	void ManageUI(float deltaTime);
};