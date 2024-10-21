#include "Texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

void Texture::Initalize(const char* name)
{
    data = stbi_load(name, &width, &height, &depth, 0);

    glGenTextures(1, &textureID);

    glBindTexture(GL_TEXTURE_2D, textureID);

    switch (depth)
    {
    case 3:
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        filterMode = TextureColorMode::RGB;
        break;

    case 4:
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        filterMode = TextureColorMode::RGBA;
        break;
    }

    switch (filterType)
    {
    case TextureFilteringType::Nearest:
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        break;

    case TextureFilteringType::Bilinear:
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        break;
    }

    fileName = name;

    glBindTexture(GL_TEXTURE_2D, 0);

    stbi_image_free(data);
}

Texture::Texture(const char* name)
{
    Initalize(name);
}

Texture::Texture(const char* name, TextureFilteringType textureType) : filterType(textureType)
{
    Initalize(name);
}

Texture::~Texture()
{
    if (!IsValid()) return;
    glDeleteTextures(1, &textureID);
}

void Texture::Bind(int textureUnit) const
{
    glActiveTexture(GL_TEXTURE0 + textureUnit);
    glBindTexture(GL_TEXTURE_2D, textureID);
}

void Texture::Unbind(int textureUnit)
{
    glActiveTexture(GL_TEXTURE0 + textureUnit);
    glBindTexture(GL_TEXTURE_2D, 0);
}