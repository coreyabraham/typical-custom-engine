#include "Utilities.h"

#include <sstream>
#include <fstream>
#include <iostream>

std::string LoadFileAsString(std::string filename)
{
	std::stringstream fileContents;
	std::ifstream fileStream(filename);

	if (!fileStream.is_open())
	{
		std::cout << "Utilities.cpp | Could not read file with filename: " << filename << ", potentially does not exist.\n";
		std::string empty;
		return empty;
	}

	while (!fileStream.eof())
	{
		std::string line;
		std::getline(fileStream, line);
		fileContents << line << "\n";
	}

	fileStream.close();

	return fileContents.str();
}

std::vector<std::string> LoadFileAsStringVector(std::string filename)
{
	std::ifstream fileStream(filename);

	std::vector <std::string> list;

	if (!fileStream.is_open())
	{
		std::cout << "Utilities.cpp | Could not read file with filename: " << filename << ", potentially does not exist.\n";
		std::string empty;
		return list;
	}

	while (!fileStream.eof())
	{
		std::string line;
		std::getline(fileStream, line);
		list.push_back(line);
	}

	fileStream.close();

	return list;
}

vec3 Float3ToVector3(float data[3])
{
	return vec3(data[0], data[1], data[2]);
}

float* Vector3ToFloat(vec3 data)
{
	return new float[3] { data.x, data.y, data.z };
}