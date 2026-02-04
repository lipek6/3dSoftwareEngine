#pragma once
#include "Vec3d.h"

struct Matrix4x4
{
	float matrix[4][4] = { 0 };
	
	Vec3d operator*(const Vec3d& inVector) const;
	Matrix4x4 operator*(const Matrix4x4& inMatrix) const;

	static Matrix4x4 MakeIdentity();
	static Matrix4x4 MakeRotationX(float fAngleRad);
	static Matrix4x4 MakeRotationY(float fAngleRad);
	static Matrix4x4 MakeRotationZ(float fAngleRad);
	static Matrix4x4 MakeTranslation(float x, float y, float z);
	static Matrix4x4 MakeProjection(float fFovDegrees, float fAspectRatio, float fNear, float fFar);
};