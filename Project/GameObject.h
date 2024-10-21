#pragma once

#include "Transform.h"

class Mesh;
class Texture;
class ShaderProgram;

// NOTE: The GameObject does NOT own textures or meshes!
class GameObject
{
private:
	Transform defaultTransform;
	float defaultGlossiness = 0.0f;
	vec3 defaultAmbientLight = vec3zero + 1.0f;
	bool defaultCastShadows = false;

public:
	Mesh* mesh;

	Texture* diffuseTexture;
	Texture* specularTexture;
	Texture* normalTexture;

	Transform transform;

	float glossiness;
	vec3 ambientLight;

	bool castShadows;

	GameObject();
	~GameObject();

	void ResetToDefaults();

	void Update(float deltaTime);
	void Draw(ShaderProgram* shader, const mat4& vpMat, vec3 camPos) const;
};