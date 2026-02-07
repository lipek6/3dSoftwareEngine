#pragma once
#include<cmath>

// 3 Dimensional Vector, with a 4th dimension for usage on matrices multiplications.
struct Vec3d
{
	float x = 0;
	float y = 0;
	float z = 0;
	float w = 1;
	
	Vec3d() = default;
	Vec3d(float _x, float _y, float _z, float _w = 1.0f);

	Vec3d operator+(const Vec3d& inVector) const;		// thisVector + inVector 
	Vec3d operator-(const Vec3d& inVector) const;		// thisVector - inVector
	Vec3d operator*(const float scalar) const;			// thisVector * scalar
	Vec3d operator/(const float scalar) const;			// thisVector / scalar

	float Length() const;
	void Normalise();
	static float DotProduct(const Vec3d& inVector1, const Vec3d& inVector2);
	static Vec3d CrossProduct(const Vec3d& inVector1, const Vec3d& inVector2);
};