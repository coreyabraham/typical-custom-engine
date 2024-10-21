#include <iostream>

#include "Mesh.h"
#include "Vertex.h"

#include "Shapes.h"
#include "Maths.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

Mesh::~Mesh()
{
	if (!IsValid()) return;
	glDeleteBuffers(1, &vertexBufferID);
}

void Mesh::CreateShape()
{
	if (shape == ShapeType::Shape_Custom) return;

	std::vector<Vertex> data;

	switch (shape)
	{
		case ShapeType::Shape_Cube: 
			data = CubeData; 
			meshName = "Cube";
			break;

		case ShapeType::Shape_Plane: 
			data = PlaneData; 
			meshName = "Plane";
			break;
	}

	isIndexed = true;
	pointCount = data.size();

	glGenBuffers(1, &vertexBufferID);

	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * data.size(), data.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, NULL);
}

void Mesh::CreateFromFile(const char* filename)
{
	Assimp::Importer importer;
	const aiScene* fileScene = importer.ReadFile(filename, aiProcess_FlipUVs | aiProcess_Triangulate | aiProcess_JoinIdenticalVertices);

	std::vector<Vertex> vertexData;
	std::vector<unsigned short> indexData;

	aiMesh* meshPointer = nullptr;

	std::string errorMsg = importer.GetErrorString();
	if (errorMsg.size() != 0)
	{
		std::cout << "Mesh failed to load with error: " << errorMsg << "\n";
		return;
	}

	if (fileScene->mNumMeshes > 0)
	{
		meshPointer = fileScene->mMeshes[0];
		std::cout << "Mesh: '" << filename << "' has " << meshPointer->mNumVertices << " vertices!\n";

		vertexData.reserve(meshPointer->mNumVertices);
		indexData.reserve(meshPointer->mNumFaces * 3);

		for (unsigned int i = 0; i < meshPointer->mNumVertices; i++)
		{
			Vertex thisVertex;

			thisVertex.position = vec3(
				meshPointer->mVertices[i].x,
				meshPointer->mVertices[i].y,
				meshPointer->mVertices[i].z
			);

			thisVertex.normal = vec3(
				meshPointer->mNormals[i].x,
				meshPointer->mNormals[i].y,
				meshPointer->mNormals[i].z
			);

			thisVertex.uv = vec2(
				meshPointer->mTextureCoords[0][i].x,
				meshPointer->mTextureCoords[0][i].y
			);

			if (meshPointer->HasTangentsAndBitangents())
			{
				thisVertex.tangent = vec4(
					meshPointer->mTangents[i].x,
					meshPointer->mTangents[i].y,
					meshPointer->mTangents[i].z,
					1.0f	//this code assumes you're *not* recreating the biteangent vector in the vert shader! (but you are, currently)
				);
			}

			else
			{
				CalculateTangents(&thisVertex, (int)indexData.size(), indexData);	//Fortunately, this is the code that's actually running, but it's a bit brittle
			}

			vertexData.push_back(thisVertex);
		}

		for (unsigned int i = 0; i < meshPointer->mNumFaces; i++)
		{
			indexData.push_back((unsigned short)meshPointer->mFaces[i].mIndices[0]);
			indexData.push_back((unsigned short)meshPointer->mFaces[i].mIndices[1]);
			indexData.push_back((unsigned short)meshPointer->mFaces[i].mIndices[2]);
		}
	}

	pointCount = (int)indexData.size();

	glGenBuffers(1, &vertexBufferID);

	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertexData.size(), vertexData.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &indexBufferID);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned short) * indexData.size(), indexData.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	meshName = filename;
	fileInitalized = true;
}

void Mesh::CalculateTangents(Vertex* vertices, unsigned int vertexCount, const std::vector<unsigned short> indices)
{
	vec4* tan1 = new vec4[vertexCount * 2];
	vec4* tan2 = tan1 + vertexCount;
	memset(tan1, 0, vertexCount * sizeof(vec4) * 2);

	unsigned int indexCount = (unsigned int)indices.size();
	for (unsigned int a = 0; a < indexCount; a += 3)
	{
		long i1 = indices[a];
		long i2 = indices[a + 1];
		long i3 = indices[a + 2];

		const vec3& v1 = vertices[i1].position;
		const vec3& v2 = vertices[i2].position;
		const vec3& v3 = vertices[i3].position;

		const vec2& w1 = vertices[i1].uv;
		const vec2& w2 = vertices[i2].uv;
		const vec2& w3 = vertices[i3].uv;

		float x1 = v2.x - v1.x;
		float x2 = v3.x - v1.x;
		float y1 = v2.y - v1.y;
		float y2 = v3.y - v1.y;
		float z1 = v2.z - v1.z;
		float z2 = v3.z - v1.z;

		float s1 = w2.x - w1.x;
		float s2 = w3.x - w1.x;
		float t1 = w2.y - w1.y;
		float t2 = w3.y - w1.y;

		float r = 1.0F / (s1 * t2 - s2 * t1);
		vec4 sdir((t2 * x1 - t1 * x2) * r, (t2 * y1 - t1 * y2) * r,
			(t2 * z1 - t1 * z2) * r, 0);
		vec4 tdir((s1 * x2 - s2 * x1) * r, (s1 * y2 - s2 * y1) * r,
			(s1 * z2 - s2 * z1) * r, 0);

		tan1[i1] += sdir;
		tan1[i2] += sdir;
		tan1[i3] += sdir;

		tan2[i1] += tdir;
		tan2[i2] += tdir;
		tan2[i3] += tdir;
	}

	for (unsigned int a = 0; a < vertexCount; a++)
	{
		const vec3& n = vec3(vertices[a].normal);
		const vec3& t = vec3(tan1[a]);

		// Gram-Schmidt orthogonalize
		vertices[a].tangent = vec4(glm::normalize(t - n * glm::dot(n,
			t)), 0);

		// Calculate handedness (direction of bitangent)
		vertices[a].tangent.w = (glm::dot(glm::cross(vec3(n),
			vec3(t)), vec3(tan2[a])) < 0.0F) ? 1.0F : -1.0F;
	}

	delete[] tan1;
}

void Mesh::Bind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
}

void Mesh::Unbind()
{
	glBindBuffer(GL_ARRAY_BUFFER, NULL);
}

void Mesh::Equip() const
{
	Bind();
	Vertex::EnableAttributes();
	Unbind();

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferID);
}

void Mesh::Unequip() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, NULL);
	Vertex::DisableAttributes();
}