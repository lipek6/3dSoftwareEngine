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
    pDepthBuffer = new float[ScreenHeight() * ScreenWidth()];

    //meshMech.LoadFromObjectFile("assets/Mech01.obj");
    //meshMech.LoadFromObjectFile("assets/UtahTeapot.obj");
    //meshMech.LoadFromObjectFile("assets/OlcAxis.obj");
    //meshMech.LoadFromObjectFile("assets/OlcMountains.obj");

    /*meshMech.vTriangle = {
    
        // SOUTH
        Triangle({ {0.0f, 0.0f, 0.0f, 1.0f},    {0.0f, 1.0f, 0.0f, 1.0f},    {1.0f, 1.0f, 0.0f, 1.0f},		{0.0f, 1.0f, 1.0f},		{0.0f, 0.0f, 1.0f},		{1.0f, 0.0f, 1.0f}, olc::WHITE}),
        Triangle({ {0.0f, 0.0f, 0.0f, 1.0f},    {1.0f, 1.0f, 0.0f, 1.0f},    {1.0f, 0.0f, 0.0f, 1.0f},		{0.0f, 1.0f, 1.0f},		{1.0f, 0.0f, 1.0f},		{1.0f, 1.0f, 1.0f}, olc::WHITE}),
    
        // EAST           																			   
        Triangle({ {1.0f, 0.0f, 0.0f, 1.0f},    {1.0f, 1.0f, 0.0f, 1.0f},    {1.0f, 1.0f, 1.0f, 1.0f},		{0.0f, 1.0f, 1.0f},		{0.0f, 0.0f, 1.0f},		{1.0f, 0.0f, 1.0f}, olc::WHITE}),
        Triangle({ {1.0f, 0.0f, 0.0f, 1.0f},    {1.0f, 1.0f, 1.0f, 1.0f},    {1.0f, 0.0f, 1.0f, 1.0f},		{0.0f, 1.0f, 1.0f},		{1.0f, 0.0f, 1.0f},		{1.0f, 1.0f, 1.0f}, olc::WHITE}),
    
        // NORTH           																			   
        Triangle({ {1.0f, 0.0f, 1.0f, 1.0f},    {1.0f, 1.0f, 1.0f, 1.0f},    {0.0f, 1.0f, 1.0f, 1.0f},		{0.0f, 1.0f, 1.0f},		{0.0f, 0.0f, 1.0f},		{1.0f, 0.0f, 1.0f}, olc::WHITE}),
        Triangle({ {1.0f, 0.0f, 1.0f, 1.0f},    {0.0f, 1.0f, 1.0f, 1.0f},    {0.0f, 0.0f, 1.0f, 1.0f},		{0.0f, 1.0f, 1.0f},		{1.0f, 0.0f, 1.0f},		{1.0f, 1.0f, 1.0f}, olc::WHITE}),

        // WEST            																			   
        Triangle({ {0.0f, 0.0f, 1.0f, 1.0f},    {0.0f, 1.0f, 1.0f, 1.0f},    {0.0f, 1.0f, 0.0f, 1.0f},		{0.0f, 1.0f, 1.0f},		{0.0f, 0.0f, 1.0f},		{1.0f, 0.0f, 1.0f}, olc::WHITE}),
        Triangle({ {0.0f, 0.0f, 1.0f, 1.0f},    {0.0f, 1.0f, 0.0f, 1.0f},    {0.0f, 0.0f, 0.0f, 1.0f},		{0.0f, 1.0f, 1.0f},		{1.0f, 0.0f, 1.0f},		{1.0f, 1.0f, 1.0f}, olc::WHITE}),

        // TOP             																			   
        Triangle({ {0.0f, 1.0f, 0.0f, 1.0f},    {0.0f, 1.0f, 1.0f, 1.0f},    {1.0f, 1.0f, 1.0f, 1.0f},		{0.0f, 1.0f, 1.0f},		{0.0f, 0.0f, 1.0f},		{1.0f, 0.0f, 1.0f}, olc::WHITE}),
        Triangle({ {0.0f, 1.0f, 0.0f, 1.0f},    {1.0f, 1.0f, 1.0f, 1.0f},    {1.0f, 1.0f, 0.0f, 1.0f},		{0.0f, 1.0f, 1.0f},		{1.0f, 0.0f, 1.0f},		{1.0f, 1.0f, 1.0f}, olc::WHITE}),

        // BOTTOM          																			  
        Triangle({ {1.0f, 0.0f, 1.0f, 1.0f},    {0.0f, 0.0f, 1.0f, 1.0f},    {0.0f, 0.0f, 0.0f, 1.0f},		{0.0f, 1.0f, 1.0f},		{0.0f, 0.0f, 1.0f},		{1.0f, 0.0f, 1.0f}, olc::WHITE}),
        Triangle({ {1.0f, 0.0f, 1.0f, 1.0f},    {0.0f, 0.0f, 0.0f, 1.0f},    {1.0f, 0.0f, 0.0f, 1.0f},		{0.0f, 1.0f, 1.0f},		{1.0f, 0.0f, 1.0f},		{1.0f, 1.0f, 1.0f}, olc::WHITE}),
    };*/


    meshMech.LoadFromObjectFile("assets/SpyroTriangulatedMap.obj", true);
    sprTexture1 = new olc::Sprite("assets/SpyroMapHigh.png");

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





    // MATRICES CREATION =====================================================================
    // fTheta += 1.0f * fElapsedTime;
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
        triTransformed.vertex [0] = matWorld * tri.vertex[0];
        triTransformed.vertex [1] = matWorld * tri.vertex[1];
        triTransformed.vertex [2] = matWorld * tri.vertex[2];

        triTransformed.texture[0] = tri.texture[0];
        triTransformed.texture[1] = tri.texture[1];
        triTransformed.texture[2] = tri.texture[2];


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
            triViewed.vertex[0] = matView * triTransformed.vertex[0];
            triViewed.vertex[1] = matView * triTransformed.vertex[1];
            triViewed.vertex[2] = matView * triTransformed.vertex[2];
            
            triViewed.color = triTransformed.color;

            triViewed.texture[0] = triTransformed.texture[0];
            triViewed.texture[1] = triTransformed.texture[1];
            triViewed.texture[2] = triTransformed.texture[2];



            // CLIPPING ----------------------------------------------------------------------
            int nClippedTriangles = 0;
            Triangle triClipped[2];
            Vec3d nearPlanePoint(0.0f, 0.0f, 0.1f);
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
                triProjected.texture[0] = triClipped[n].texture[0];
                triProjected.texture[1] = triClipped[n].texture[1];
                triProjected.texture[2] = triClipped[n].texture[2];

                triProjected.texture[0].u = triProjected.texture[0].u / triProjected.vertex[0].w;
                triProjected.texture[1].u = triProjected.texture[1].u / triProjected.vertex[1].w;
                triProjected.texture[2].u = triProjected.texture[2].u / triProjected.vertex[2].w;

                triProjected.texture[0].v = triProjected.texture[0].v / triProjected.vertex[0].w;
                triProjected.texture[1].v = triProjected.texture[1].v / triProjected.vertex[1].w;
                triProjected.texture[2].v = triProjected.texture[2].v / triProjected.vertex[2].w;

                triProjected.texture[0].w = 1.0f / triProjected.vertex[0].w;
                triProjected.texture[1].w = 1.0f / triProjected.vertex[1].w;
                triProjected.texture[2].w = 1.0f / triProjected.vertex[2].w;



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

    

    // PAINTERS ALGORITHM ====================================================================    REMOVED FOR A DEPTH BUFFFER
    //std::sort(vTrianglesToRasterize.begin(), vTrianglesToRasterize.end(), [](const Triangle& triangle0, const Triangle& triangle1)
    //    {
    //        float midPointZ0 = (triangle0.vertex[0].z + triangle0.vertex[1].z + triangle0.vertex[2].z) / 3.0f;
    //        float midPointZ1 = (triangle1.vertex[0].z + triangle1.vertex[1].z + triangle1.vertex[2].z) / 3.0f;
    //
    //        return midPointZ0 > midPointZ1;         
    //    });





    // RENDER CLEANING ==========================================================================
    Clear({ 135, 206, 235 });
    for (int i = 0; i < ScreenWidth() * ScreenHeight(); i++)
        pDepthBuffer[i] = 0.0f;





    // RASTERIZATION =========================================================================
    for (auto& triToRaster : vTrianglesToRasterize)
    {
        // CLIPPING --------------------------------------------------------------------------
        Triangle clipped[2];
        std::list<Triangle> listTriangles;
        listTriangles.push_back(triToRaster);
        int nNewTriangles = 1;
        
        for (int plane = 0; plane < 4; plane++)
        {
            int nTrisToAdd = 0;
            while (nNewTriangles > 0)
            {
                Triangle test = listTriangles.front();
                listTriangles.pop_front();
                nNewTriangles--;

                switch (plane)
                {
                case 0: nTrisToAdd = Triangle::ClipAgainstPlane({ 0.0f, 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f }, test, clipped[0], clipped[1]); break;
                case 1: nTrisToAdd = Triangle::ClipAgainstPlane({ 0.0f, (float)ScreenHeight() - 1, 0.0f }, { 0.0f, -1.0f, 0.0f }, test, clipped[0], clipped[1]); break;
                case 2: nTrisToAdd = Triangle::ClipAgainstPlane({ 0.0f, 0.0f, 0.0f }, { 1.0f, 0.0f, 0.0f }, test, clipped[0], clipped[1]); break;
                case 3: nTrisToAdd = Triangle::ClipAgainstPlane({ (float)ScreenWidth() - 1, 0.0f, 0.0f }, { -1.0f, 0.0f, 0.0f }, test, clipped[0], clipped[1]); break;
                }

                for (int w = 0; w < nTrisToAdd; w++)
                    listTriangles.push_back(clipped[w]);
            }
            nNewTriangles = listTriangles.size();
        }



        // DRAW ------------------------------------------------------------------------------
        for (const Triangle& tri : listTriangles)
        {
            TexturedTriangle(tri.vertex[0].x, tri.vertex[0].y, tri.texture[0].u, tri.texture[0].v, tri.texture[0].w,
                             tri.vertex[1].x, tri.vertex[1].y, tri.texture[1].u, tri.texture[1].v, tri.texture[1].w,
                             tri.vertex[2].x, tri.vertex[2].y, tri.texture[2].u, tri.texture[2].v, tri.texture[2].w, 
                             sprTexture1);

            // FillTriangle(tri.vertex[0].x, tri.vertex[0].y, tri.vertex[1].x, tri.vertex[1].y, tri.vertex[2].x, tri.vertex[2].y, tri.color);
            // DrawTriangle(tri.vertex[0].x, tri.vertex[0].y, tri.vertex[1].x, tri.vertex[1].y, tri.vertex[2].x, tri.vertex[2].y, olc::WHITE);
        }
    }

    return true;
}



void Engine3d::TexturedTriangle(int x1, int y1, float u1, float v1, float w1,
                                int x2, int y2, float u2, float v2, float w2,
                                int x3, int y3, float u3, float v3, float w3,
    olc::Sprite* texture)
{
    // Sorting y1(highest y=0), y2(middle), y3(lowest y=1)
    if (y2 < y1)
    {
        std::swap(y1, y2);
        std::swap(x1, x2);
        std::swap(u1, u2);
        std::swap(v1, v2);
        std::swap(w1, w2);
    }

    if (y3 < y1)
    {
        std::swap(y1, y3);
        std::swap(x1, x3);
        std::swap(u1, u3);
        std::swap(v1, v3);
        std::swap(w1, w3);
    }

    if (y3 < y2)
    {
        std::swap(y2, y3);
        std::swap(x2, x3);
        std::swap(u2, u3);
        std::swap(v2, v3);
        std::swap(w2, w3);
    }

    int   dy1 = y2 - y1;
    int   dx1 = x2 - x1;
    float dv1 = v2 - v1;
    float du1 = u2 - u1;
    float dw1 = w2 - w1;

    int   dy2 = y3 - y1;
    int   dx2 = x3 - x1;
    float dv2 = v3 - v1;
    float du2 = u3 - u1;
    float dw2 = w3 - w1;

    float tex_u, tex_v, tex_w;

    float dax_step = 0, dbx_step = 0,
          du1_step = 0, dv1_step = 0,
          du2_step = 0, dv2_step = 0,
          dw1_step = 0, dw2_step = 0;

    if (dy1) dax_step = dx1 / (float)abs(dy1);
    if (dy2) dbx_step = dx2 / (float)abs(dy2);

    if (dy1) du1_step = du1 / (float)abs(dy1);
    if (dy1) dv1_step = dv1 / (float)abs(dy1);
    if (dy1) dw1_step = dw1 / (float)abs(dy1);

    if (dy2) du2_step = du2 / (float)abs(dy2);
    if (dy2) dv2_step = dv2 / (float)abs(dy2);
    if (dy2) dw2_step = dw2 / (float)abs(dy2);

    if (dy1)
    {
        for (int i = y1; i <= y2; i++)
        {
            int ax = x1 + (float)(i - y1) * dax_step;
            int bx = x1 + (float)(i - y1) * dbx_step;

            float tex_su = u1 + (float)(i - y1) * du1_step;
            float tex_sv = v1 + (float)(i - y1) * dv1_step;
            float tex_sw = w1 + (float)(i - y1) * dw1_step;

            float tex_eu = u1 + (float)(i - y1) * du2_step;
            float tex_ev = v1 + (float)(i - y1) * dv2_step;
            float tex_ew = w1 + (float)(i - y1) * dw2_step;

            if (ax > bx)
            {
                std::swap(ax, bx);
                std::swap(tex_su, tex_eu);
                std::swap(tex_sv, tex_ev);
                std::swap(tex_sw, tex_ew);
            }

            tex_u = tex_su;
            tex_v = tex_sv;
            tex_w = tex_sw;

            float tstep = 1.0f / ((float)(bx - ax));
            float t = 0.0f;

            for (int j = ax; j < bx; j++)
            {
                tex_u = (1.0f - t) * tex_su + t * tex_eu;
                tex_v = (1.0f - t) * tex_sv + t * tex_ev;
                tex_w = (1.0f - t) * tex_sw + t * tex_ew;

                if (tex_w > pDepthBuffer[i * ScreenWidth() + j])
                {
                    Draw(j, i, texture->Sample(tex_u / tex_w, tex_v / tex_w));
                    pDepthBuffer[i * ScreenWidth() + j] = tex_w;
                }
                
                t += tstep;
            }

        }
    }

    dy1 = y3 - y2;
    dx1 = x3 - x2;
    dv1 = v3 - v2;
    du1 = u3 - u2;
    dw1 = w3 - w2;

    if (dy1) dax_step = dx1 / (float)abs(dy1);
    if (dy2) dbx_step = dx2 / (float)abs(dy2);

    du1_step = 0, dv1_step = 0;
    if (dy1) du1_step = du1 / (float)abs(dy1);
    if (dy1) dv1_step = dv1 / (float)abs(dy1);
    if (dy1) dw1_step = dw1 / (float)abs(dy1);

    if (dy1)
    {
        for (int i = y2; i <= y3; i++)
        {
            int ax = x2 + (float)(i - y2) * dax_step;
            int bx = x1 + (float)(i - y1) * dbx_step;

            float tex_su = u2 + (float)(i - y2) * du1_step;
            float tex_sv = v2 + (float)(i - y2) * dv1_step;
            float tex_sw = w2 + (float)(i - y2) * dw1_step;

            float tex_eu = u1 + (float)(i - y1) * du2_step;
            float tex_ev = v1 + (float)(i - y1) * dv2_step;
            float tex_ew = w1 + (float)(i - y1) * dw2_step;

            if (ax > bx)
            {
                std::swap(ax, bx);
                std::swap(tex_su, tex_eu);
                std::swap(tex_sv, tex_ev);
                std::swap(tex_sw, tex_ew);
            }

            tex_u = tex_su;
            tex_v = tex_sv;
            tex_w = tex_sw;

            float tstep = 1.0f / ((float)(bx - ax));
            float t = 0.0f;

            for (int j = ax; j < bx; j++)
            {
                tex_u = (1.0f - t) * tex_su + t * tex_eu;
                tex_v = (1.0f - t) * tex_sv + t * tex_ev;
                tex_w = (1.0f - t) * tex_sw + t * tex_ew;

                if (tex_w > pDepthBuffer[i * ScreenWidth() + j])
                {
                    Draw(j, i, texture->Sample(tex_u / tex_w, tex_v / tex_w));
                    pDepthBuffer[i * ScreenWidth() + j] = tex_w;
                }
                t += tstep;
            }
        }
    }
}