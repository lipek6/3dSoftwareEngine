#pragma once
#include "olcPixelGameEngine.h"
#include "Matrix4x4.h"
#include "Vec3d.h"
#include "Mesh.h"

#include <algorithm>

class Engine3d : public olc::PixelGameEngine
{
private:
    Mesh meshMech;
    Matrix4x4 matProjection;
    Vec3d vCamera;
    Vec3d vLookDir;
    float fYaw = 0.0f;
    float fTheta = 0.0f;

public:
    Engine3d();

    bool OnUserCreate() override;
    bool OnUserUpdate(float fElapsedTime) override;
    olc::Pixel GetColor(float lum);
};