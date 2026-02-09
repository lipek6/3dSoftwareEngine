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
    //meshMech.LoadFromObjectFile("Mech01.obj");
    //meshMech.LoadFromObjectFile("UtahTeapot.obj");
    meshMech.LoadFromObjectFile("OlcAxis.obj");

    matProjection = Matrix4x4::MakeProjection(90.0f, (float)ScreenHeight() / (float)ScreenWidth(), 0.1f, 1000.0f);

    return true;
}



bool Engine3d::OnUserUpdate(float fElapsedTime)
{
    // USER INPUT ===============================================================================================================
    if (GetKey(olc::Key::UP).bHeld)
        vCamera.y += 8.0f * fElapsedTime;

    if (GetKey(olc::Key::DOWN).bHeld)
        vCamera.y -= 8.0f * fElapsedTime;
    
    if (GetKey(olc::Key::LEFT).bHeld)           // INVERSED !?
        vCamera.x += 8.0f * fElapsedTime;
        
    if (GetKey(olc::Key::RIGHT).bHeld)
        vCamera.x -= 8.0f * fElapsedTime;

    Vec3d vForward = vLookDir * (8.0f * fElapsedTime);
    
    if (GetKey(olc::Key::A).bHeld)
        fYaw -= 2.0f * fElapsedTime;

    if (GetKey(olc::Key::D).bHeld)
        fYaw += 2.0f * fElapsedTime;
  

    if (GetKey(olc::Key::W).bHeld)
        vCamera = vCamera + vForward;

    if (GetKey(olc::Key::S).bHeld)
        vCamera = vCamera - vForward;



    // RENDER SETUP ==========================================================================
    Clear(olc::BLACK);



    // MATRICES CREATION =====================================================================
    Matrix4x4 matRotateZ   = Matrix4x4::MakeRotationZ(fTheta);
    Matrix4x4 matRotateX   = Matrix4x4::MakeRotationX(fTheta * 0.5f);
    Matrix4x4 matTranslate = Matrix4x4::MakeTranslation(0.0f, 0.0f, 8.0f);
    Matrix4x4 matWorld = matRotateZ * matRotateX * matTranslate; 

    Vec3d vUp(0.0f, 1.0f, 0.0f);
    Vec3d vTarget = { 0.0f, 0.0f, 1.0f };

    Matrix4x4 matCameraRot = Matrix4x4::MakeRotationY(fYaw);
    vLookDir = matCameraRot * vTarget;
    vTarget = vCamera + vLookDir;

    Matrix4x4 matCamera = Matrix4x4::MakePointAt(vCamera, vTarget, vUp);
    Matrix4x4 matView   = Matrix4x4::MakeQuickInverse(matCamera);



    // RASTERIZATION PREPARATION =============================================================
    std::vector<Triangle> vTrianglesToRasterize;
    for (Triangle& tri : meshMech.vTriangle)
    {
        // POSITIONING -----------------------------------------------------------------------
        Triangle triTransformed, triProjected, triViewed;
        triTransformed.vertex[0] = matWorld * tri.vertex[0];
        triTransformed.vertex[1] = matWorld * tri.vertex[1];
        triTransformed.vertex[2] = matWorld * tri.vertex[2];

        Vec3d line1 = triTransformed.vertex[1] - triTransformed.vertex[0];
        Vec3d line2 = triTransformed.vertex[2] - triTransformed.vertex[0];
        Vec3d normal = Vec3d::CrossProduct(line1, line2);
        normal.Normalise();

        Vec3d vCameraRay = triTransformed.vertex[0] - vCamera;



        // VISIBLE TRIANGLES -----------------------------------------------------------------
        if (Vec3d::DotProduct(normal, vCameraRay) < 0.0f)
        {
            // LIGTHING ----------------------------------------------------------------------
            Vec3d ligthDirection(0.0f, 1.0f, -1.0f);
            ligthDirection.Normalise();

            float dpLigthNormal = std::max(0.1f, Vec3d::DotProduct(ligthDirection, normal));
            triTransformed.color = GetColor(dpLigthNormal);



            // WORLD TO VIEW CONVERSION ------------------------------------------------------
            triViewed.vertex[0] = matView * triTransformed.vertex[0];
            triViewed.vertex[1] = matView * triTransformed.vertex[1];
            triViewed.vertex[2] = matView * triTransformed.vertex[2];
            triViewed.color = triTransformed.color;



            // CLIPPING ----------------------------------------------------------------------
            int nClippedTriangles = 0;
            Triangle triClipped[2];
            Vec3d nearPlanePoint(0.0f, 0.0f, 1.1f);
            Vec3d nearPlaneNormal(0.0f, 0.0f, 1.0f);

            nClippedTriangles = Triangle::ClipAgainstPlane(nearPlanePoint, nearPlaneNormal, triViewed, triClipped[0], triClipped[1]); // Z CLIP

            for (int n = 0; n < nClippedTriangles; n++)
            {
                // 3D TO 2D CONVERSION
                Triangle triProjected;
                triProjected.vertex[0] = matProjection * triClipped[n].vertex[0];
                triProjected.vertex[1] = matProjection * triClipped[n].vertex[1];
                triProjected.vertex[2] = matProjection * triClipped[n].vertex[2];
                triProjected.color = triClipped[n].color;

                // VIEW SCALING
                triProjected.vertex[0] = triProjected.vertex[0] / triProjected.vertex[0].w;
                triProjected.vertex[1] = triProjected.vertex[1] / triProjected.vertex[1].w;
                triProjected.vertex[2] = triProjected.vertex[2] / triProjected.vertex[2].w;

                // INVERSING THE INVERSED
                triProjected.vertex[0].x *= -1.0f;
                triProjected.vertex[1].x *= -1.0f;
                triProjected.vertex[2].x *= -1.0f;
                triProjected.vertex[0].y *= -1.0f;
                triProjected.vertex[1].y *= -1.0f;
                triProjected.vertex[2].y *= -1.0f;

                // OFFSET INTO VISIBLE NORMALISED SPACE
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

                // STORE THE TRIANGLE
                vTrianglesToRasterize.push_back(triProjected);
            }
        }
    }

    
    // PAINTERS ALGORITHM ====================================================================    
    std::sort(vTrianglesToRasterize.begin(), vTrianglesToRasterize.end(), [](const Triangle& triangle0, const Triangle& triangle1)
        {
            float midPointZ0 = (triangle0.vertex[0].z + triangle0.vertex[1].z + triangle0.vertex[2].z) / 3.0f;
            float midPointZ1 = (triangle1.vertex[0].z + triangle1.vertex[1].z + triangle1.vertex[2].z) / 3.0f;

            return midPointZ0 > midPointZ1;         
        });


    // RASTERIZATION =========================================================================
    for (auto& triToRaster : vTrianglesToRasterize)
    {
        FillTriangle(triToRaster.vertex[0].x, triToRaster.vertex[0].y,
            triToRaster.vertex[1].x, triToRaster.vertex[1].y,
            triToRaster.vertex[2].x, triToRaster.vertex[2].y,
            triToRaster.color);

        DrawTriangle(triToRaster.vertex[0].x, triToRaster.vertex[0].y,
            triToRaster.vertex[1].x, triToRaster.vertex[1].y,
            triToRaster.vertex[2].x, triToRaster.vertex[2].y,
            olc::BLACK);

    }

    return true;
}