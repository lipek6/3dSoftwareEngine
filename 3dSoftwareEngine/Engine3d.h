#pragma once
#include "olcPixelGameEngine.h"
#include "Matrix4x4.h"
#include "Vec3d.h"
#include "Vec2d.h"
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

    olc::Sprite* sprTexture1;

public:
    Engine3d();

    bool OnUserCreate() override;
    bool OnUserUpdate(float fElapsedTime) override;


    void TexturedTriangle(int x1, int y1, float u1, float v1, float w1,
                          int x2, int y2, float u2, float v2, float w2,
                          int x3, int y3, float u3, float v3, float w3,
                          olc::Sprite* texture);

    olc::Pixel GetColor(float lum);
};


