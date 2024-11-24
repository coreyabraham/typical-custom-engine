#pragma once

#include <string>
#include <vector>

#include "Maths.h"

bool DoesFileExist(std::string filename);

std::string LoadFileAsString(std::string filename);
std::vector<std::string> LoadFileAsStringVector(std::string filename);

vec3 Float3ToVector3(float data[3]);
float* Vector3ToFloat(vec3 data);