#pragma once

#include <math.h>

#include "glm.hpp"
#include <ext.hpp>

typedef glm::vec2 vec2;
typedef glm::vec3 vec3;
typedef glm::vec4 vec4;

typedef glm::mat4 mat4;

#define PI 3.14159265f

template <typename T>
T Lerp(T a, T b, float t)
{
	return a * (1.0f - t) + b * t;
}