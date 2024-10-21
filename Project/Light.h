#pragma once

#include "Maths.h"

constexpr unsigned int SHADER_LIGHT_CAP = 100;

class Light
{
public:
	// TODO: TEMPORARY CONSTRUCTOR AND DECONSTRUCTOR! PLEASE REPLACE!
	Light() = default;
	~Light() = default;

	vec3 direction = vec3(0);
	vec3 colour = vec3(1, 1, 1);
};