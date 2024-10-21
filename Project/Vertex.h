#pragma once

#include "Maths.h"

class Vertex
{
public:
	vec3 position;
	vec3 normal;
	vec2 uv;
	vec4 tangent;

	static void EnableAttributes();
	static void DisableAttributes();
};