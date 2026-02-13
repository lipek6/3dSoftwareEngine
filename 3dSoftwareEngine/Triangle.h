#pragma once
#include "olcPixelGameEngine.h"
#include "Vec3d.h"
#include "Vec2d.h"
#include <algorithm>

struct Triangle
{
	Vec3d vertex[3];
	Vec2d texture[3];
	olc::Pixel color;

	Triangle() = default;
	Triangle(Vec3d v1, Vec3d v2, Vec3d v3, Vec2d t1, Vec2d t2, Vec2d t3, olc::Pixel col);
	static int ClipAgainstPlane(Vec3d planePoint, Vec3d planeNormal, Triangle &inTriangle, Triangle& outTriangle1, Triangle& outTriangle2);
};