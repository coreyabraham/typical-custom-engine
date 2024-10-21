#pragma once

#include "Graphics.h"
#include "Maths.h"

enum TextureFilteringType
{
	Nearest = 0,
	Bilinear
};

enum TextureColorMode
{
	RGB = 0,
	RGBA
};

class Texture
{
private:
	GLuint textureID = 0;

	int width;
	int height;
	int depth;

	unsigned char* data;
	const char* fileName;

	void Initalize(const char* name);

public:
	TextureFilteringType filterType = TextureFilteringType::Nearest;
	TextureColorMode filterMode;

	Texture() = default;
	Texture(const char* name);
	Texture(const char* name, TextureFilteringType textureType);

	~Texture();

	Texture(const Texture& other) = delete;
	Texture& operator=(const Texture& other) = delete;

	void Bind(int textureUnit = 0) const;
	static void Unbind(int textureUnit = 0);

	int GetWidth() const { return width; }
	int GetHeight() const { return height; }
	int GetDepth() const { return depth; }

	GLuint GetTextureID() const { return textureID; }
	const char* GetFileName() const { return fileName; }

	bool IsValid() const { return textureID != 0; }
};