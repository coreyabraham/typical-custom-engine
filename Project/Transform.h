#pragma once

#include "Maths.h"

struct Transform
{
public:
	vec3 position = vec3zero;
	
	// Eular Angles || Rotation Order: X / Y / Z
	vec3 rotation = vec3zero;
	
	vec3 size = vec3zero;
};