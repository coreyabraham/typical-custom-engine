#pragma once

#include "Graphics.h"

class Vertex;

enum ShapeType
{
	Shape_Custom = 0,
	Shape_Cube,
	Shape_Plane
};

class Mesh
{
private:
	GLuint vertexBufferID = 0;
	GLuint indexBufferID = 0;
	int pointCount = 0;

	ShapeType shape = ShapeType::Shape_Custom;

	void Bind() const;
	static void Unbind();

	bool isIndexed = false;
	bool fileInitalized = false;

	const char* meshName = NULL;

public:
	Mesh() = default;
	~Mesh();

	void CreateShape();

	ShapeType GetShape() const { return shape; }
	void SetShape(ShapeType Shape) { shape = Shape; }

	void CreateFromFile(const char* filename);
	int GetPointCount() const { return pointCount; }

	void Equip() const;
	void Unequip() const;

	bool IsValid() const { return vertexBufferID != 0; }
	bool IsIndexed() const { return isIndexed; }
	bool HasFileInitalized() const { return fileInitalized; }

	void CalculateTangents(Vertex* vertices, unsigned int vertexCount, const std::vector<unsigned short> indices);

	const char* GetFileName() const { return meshName; }
};
