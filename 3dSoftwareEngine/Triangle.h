#pragma once
#include "olcPixelGameEngine.h"
#include "Vec3d.h"

struct Triangle
{
	Vec3d vertex[3];
	olc::Pixel color;

	static int ClipAgainstPlane(Vec3d& planePoint, Vec3d& planeNormal, Triangle &inTriangle, Triangle& outTriangle1, Triangle& outTriangle2);
};