#include "Mesh.h"
#include "Texture.h"
#include "ShaderProgram.h"

#include "GameObject.h"

GameObject::GameObject()
{
	transform = defaultTransform;
	glossiness = defaultGlossiness;
	ambientLight = defaultAmbientLight;
	castShadows = defaultCastShadows;
}

GameObject::~GameObject()
{

}

void GameObject::ResetToDefaults()
{
	transform = defaultTransform;
	glossiness = defaultGlossiness;
	ambientLight = defaultAmbientLight;
	castShadows = defaultCastShadows;
}

void GameObject::Update(float deltaTime)
{

}

void GameObject::Draw(ShaderProgram* shader, const mat4& vpMat, vec3 camPos) const
{
	mat4 modelMatrix(1);	// Initialise as the identity matrix

	modelMatrix = glm::rotate(modelMatrix, transform.rotation.x, vec3(1, 0, 0));
	modelMatrix = glm::rotate(modelMatrix, transform.rotation.y, vec3(0, 1, 0));
	modelMatrix = glm::rotate(modelMatrix, transform.rotation.z, vec3(0, 0, 1));

	modelMatrix = glm::translate(modelMatrix, transform.position);

	mat4 mvpMatrix = vpMat * modelMatrix;

	shader->SetFloatUniform("glossiness", glossiness);
	shader->SetVec3Uniform("ambientLight", ambientLight);

	shader->SetMat4Uniform("mvpMatrix", &mvpMatrix[0][0]);
	shader->SetMat4Uniform("modelMatrix", &modelMatrix[0][0]);

	shader->SetIntUniform("diffuseSampler", 0);
	shader->SetIntUniform("specularSampler", 1);
	shader->SetIntUniform("normalSampler", 2);

	if (diffuseTexture != nullptr) diffuseTexture->Bind(0);
	if (specularTexture != nullptr) specularTexture->Bind(1);
	if (normalTexture != nullptr) normalTexture->Bind(2);

	mesh->Equip();

	if (mesh->IsIndexed()) glDrawArrays(GL_TRIANGLES, NULL, mesh->GetPointCount());
	else glDrawElements(GL_TRIANGLES, mesh->GetPointCount(), GL_UNSIGNED_SHORT, 0);

	int textureUnit = 0;
	if (diffuseTexture != nullptr) textureUnit++;
	if (specularTexture != nullptr) textureUnit++;
	if (normalTexture != nullptr) textureUnit++;

	Texture::Unbind(textureUnit);

	mesh->Unequip();
}
