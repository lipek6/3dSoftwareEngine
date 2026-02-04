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

Vec3d Vec3d::Normalise() const
{
	float length = Length();

	if (length == 0)
		return { 0.0f, 0.0f, 0.0f };

	return { x/length, y/ length, z/ length };
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