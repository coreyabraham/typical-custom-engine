#include "Utilities.h"

#include <sstream>

#include <iostream>
#include <fstream>

std::string LoadFileAsString(std::string fileName)
{
	std::stringstream fileContents;
	std::ifstream fileStream(fileName);

	if (!fileStream.is_open())
	{
		std::cout << "Utilities.cpp | Could not read file with fileName: " << fileName << ", potentially does not exist.\n";
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