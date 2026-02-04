#pragma once
#include "Triangle.h"
#include <string>
#include <vector>
#include <fstream>
#include <sstream>

#define MAX_INLINE_CHARS_OBJFILE 128

struct Mesh
{
	std::vector<Triangle> vTriangle;

	bool LoadFromObjectFile(std::string sFilename);
};