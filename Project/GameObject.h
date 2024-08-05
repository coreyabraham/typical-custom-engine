#pragma once

#include "Maths.h"

class Mesh;
class Texture;
class ShaderProgram;

enum GameObject_Animation
{
	Static = 0,
	YSpin,
	YSwivel
};

struct ImGui_GameObject
{
	//size_t maxStringSize = 256;

	//char mesh[256];

	//char diffuseTexture[256];
	//char specularTexture[256];
	//char normalTexture[256];

	float glossiness;
	float ambientLight[3];

	float position[3];
	float rotation[3];
};

class GameObject
{
private:
	bool defaultsInitalized;

	vec3 defaultPosition;
	vec3 defaultRotation;

	GameObject_Animation defaultAnimation;

	float defaultGlossiness;
	vec3 defaultAmbientLight;

public:
	// NOTE: The GameObject does NOT own textures or meshes!
	Mesh* mesh;

	Texture* diffuseTexture;
	Texture* specularTexture;
	Texture* normalTexture;

	vec3 position;

	// Eular Angles,
	// x/y/z rotation order.
	vec3 rotation;

	float glossiness;
	vec3 ambientLight;

	GameObject_Animation animation;

	void SetupDefaults();
	void ResetToDefaults();

	void Update(float deltaTime);
	void Draw(ShaderProgram* shader, const mat4& vpMat, vec3 camPos) const;
};