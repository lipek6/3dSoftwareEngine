#include "Engine3d.h"

Engine3d::Engine3d()
{
    sAppName = "3D Demo";
}

olc::Pixel Engine3d::GetColor(float lum)
{
    int nValue = (int)(std::max(lum, 0.20f) * 255.0f);
    return olc::Pixel(nValue, nValue, nValue);
}



bool Engine3d::OnUserCreate()
{
    meshMech.LoadFromObjectFile("Mech01_Official01.obj");

    matProjection = Matrix4x4::MakeProjection(90.0f, (float)ScreenHeight() / (float)ScreenWidth(), 0.1f, 1000.0f);

    return true;
}



bool Engine3d::OnUserUpdate(float fElapsedTime)
{
    Clear(olc::BLACK);
    fTheta += 1.0f * fElapsedTime;

    Matrix4x4 matRotateZ   = Matrix4x4::MakeRotationZ(fTheta);
    Matrix4x4 matRotateX   = Matrix4x4::MakeRotationX(fTheta * 0.5f);
    Matrix4x4 matTranslate = Matrix4x4::MakeTranslation(0.0f, 0.0f, 80.0f);

    Matrix4x4 matWorld = matRotateZ * matRotateX * matTranslate;    // ERROR POSSIBLE


    std::vector<Triangle> vTrianglesToRasterize;
    for (Triangle& tri : meshMech.vTriangle)
    {
        Triangle triTransformed, triProjected;

        triTransformed.vertex[0] = matWorld * tri.vertex[0];
        triTransformed.vertex[1] = matWorld * tri.vertex[1];
        triTransformed.vertex[2] = matWorld * tri.vertex[2];

        Vec3d line1 = triTransformed.vertex[1] - triTransformed.vertex[0];
        Vec3d line2 = triTransformed.vertex[2] - triTransformed.vertex[0];
        Vec3d normal = Vec3d::CrossProduct(line1, line2);
        normal = normal.Normalise();

        Vec3d vCameraRay = triTransformed.vertex[0] - vCamera;

        if (Vec3d::DotProduct(normal, vCameraRay) < 0.0f)
        {
            Vec3d ligthDirection(0.0f, 1.0f, -1.0f);
            ligthDirection = ligthDirection.Normalise();

            float dpLigthNormal = std::max(0.1f, Vec3d::DotProduct(ligthDirection, normal));
            triTransformed.color = GetColor(dpLigthNormal);


            Triangle triProjected;
            triProjected.vertex[0] = matProjection * triTransformed.vertex[0];
            triProjected.vertex[1] = matProjection * triTransformed.vertex[1];
            triProjected.vertex[2] = matProjection * triTransformed.vertex[2];
            triProjected.color = triTransformed.color;

            triProjected.vertex[0] = triProjected.vertex[0] / triProjected.vertex[0].w;
            triProjected.vertex[1] = triProjected.vertex[1] / triProjected.vertex[1].w;
            triProjected.vertex[2] = triProjected.vertex[2] / triProjected.vertex[2].w;



            Vec3d vOffsetView(1.0f, 1.0f, 0.0f);
            triProjected.vertex[0] = triProjected.vertex[0] + vOffsetView;
            triProjected.vertex[1] = triProjected.vertex[1] + vOffsetView;
            triProjected.vertex[2] = triProjected.vertex[2] + vOffsetView;

            triProjected.vertex[0].x *= 0.5f * (float)ScreenWidth();
            triProjected.vertex[0].y *= 0.5f * (float)ScreenHeight();

            triProjected.vertex[1].x *= 0.5f * (float)ScreenWidth();
            triProjected.vertex[1].y *= 0.5f * (float)ScreenHeight();

            triProjected.vertex[2].x *= 0.5f * (float)ScreenWidth();
            triProjected.vertex[2].y *= 0.5f * (float)ScreenHeight();



            vTrianglesToRasterize.push_back(triProjected);
        }
    }

    
    
    std::sort(vTrianglesToRasterize.begin(), vTrianglesToRasterize.end(), [](const Triangle& triangle0, const Triangle& triangle1)
        {
            float midPointZ0 = (triangle0.vertex[0].z + triangle0.vertex[1].z + triangle0.vertex[2].z) / 3.0f;
            float midPointZ1 = (triangle1.vertex[0].z + triangle1.vertex[1].z + triangle1.vertex[2].z) / 3.0f;

            return midPointZ0 > midPointZ1;         
        });



    for (auto& triProjected : vTrianglesToRasterize)
    {
        FillTriangle(triProjected.vertex[0].x, triProjected.vertex[0].y,
            triProjected.vertex[1].x, triProjected.vertex[1].y,
            triProjected.vertex[2].x, triProjected.vertex[2].y,
            triProjected.color);

        //DrawTriangle(triProjected.vertex[0].x, triProjected.vertex[0].y,
        //    triProjected.vertex[1].x, triProjected.vertex[1].y,
        //    triProjected.vertex[2].x, triProjected.vertex[2].y,
        //    olc::BLACK);

    }

    return true;
}