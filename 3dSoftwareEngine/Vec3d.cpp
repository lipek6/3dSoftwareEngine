#include "Vec3d.h"

// Constructor ===============================================================================
Vec3d::Vec3d(float _x, float _y, float _z, float _w)
	: x(_x), y(_y), z(_z), w(_w) {}

// Operators =================================================================================
Vec3d Vec3d::operator+(const Vec3d& inVector) const
{
	return { x + inVector.x, y + inVector.y, z + inVector.z };
}

Vec3d Vec3d::operator-(const Vec3d& inVector) const
{
	return { x - inVector.x, y - inVector.y, z - inVector.z };
}

Vec3d Vec3d::operator*(const float scalar) const
{
	return { x * scalar, y * scalar, z * scalar };
}

Vec3d Vec3d::operator/(const float scalar) const
{
	return { x / scalar, y / scalar, z / scalar };
}

// Utility Functions =========================================================================
float Vec3d::Length() const
{
	return std::sqrtf(DotProduct(*this, *this));
}

void Vec3d::Normalise()
{
	float length = Length();
	
	if (length == 0) return;

	(*this) = (*this) / length;
}

float Vec3d::DotProduct(const Vec3d& inVector1, const Vec3d& inVector2)
{
	return inVector1.x * inVector2.x + inVector1.y * inVector2.y + inVector1.z * inVector2.z;
}

Vec3d Vec3d::CrossProduct(const Vec3d& inVector1, const Vec3d& inVector2)
{
	Vec3d outVector;
	outVector.x = inVector1.y * inVector2.z - inVector1.z * inVector2.y;
	outVector.y = inVector1.z * inVector2.x - inVector1.x * inVector2.z;
	outVector.z = inVector1.x * inVector2.y - inVector1.y * inVector2.x;
	return outVector;
}

Vec3d Vec3d::IntersectPlane(Vec3d& planePoint, Vec3d& planeNormal, const Vec3d& lineStart, const Vec3d& lineEnd, float& t)
{
	planeNormal.Normalise();
	
	float planeDot = -(Vec3d::DotProduct(planeNormal, planePoint));	
	float projectedStart = Vec3d::DotProduct(lineStart, planeNormal);
	float projectedEnd = Vec3d::DotProduct(lineEnd, planeNormal);
	
	t = (-planeDot - projectedStart) / (projectedEnd - projectedStart);

	Vec3d lineStartToEnd = lineEnd - lineStart;
	Vec3d lineToIntersect = lineStartToEnd * t;

	return lineStart + lineToIntersect;
}