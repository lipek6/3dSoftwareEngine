#include "olcConsoleGameEngine.h"

// STRUCTS ===================================================================================

// 3 Dimensional vector.
struct Vec3d
{
    float x, y, z;
};


// Set of 3 Dimensional vectors used to represent triangles.
struct Triangle
{
    Vec3d vertex[3];
};


// Set of triangles used to Represent the objects.
struct Mesh             
{
    std::vector<Triangle> vTriangle;
};


// 4 by 4 Matrix ordered by row followed by collum.
struct Matrix4x4
{
    float matrix[4][4] = { 0 };
};



// GAME ENGINE CLASS =========================================================================
class Engine3D : public olcConsoleGameEngine
{
private:
    
    Mesh meshCube;
    Matrix4x4 matProjection;

    // Multiply a 3D vector with a 4x4 matrix and writes the result into outVector 
    void MultiplyMatrixVector(Vec3d &inVector, Vec3d &outVector, Matrix4x4 &inMatrix)
    {
        outVector.x = (inVector.x * inMatrix.matrix[0][0]) + (inVector.y * inMatrix.matrix[1][0]) + (inVector.z * inMatrix.matrix[2][0]) + (inMatrix.matrix[3][0]);
        outVector.y = (inVector.x * inMatrix.matrix[0][1]) + (inVector.y * inMatrix.matrix[1][1]) + (inVector.z * inMatrix.matrix[2][1]) + (inMatrix.matrix[3][1]);
        outVector.z = (inVector.x * inMatrix.matrix[0][2]) + (inVector.y * inMatrix.matrix[1][2]) + (inVector.z * inMatrix.matrix[2][2]) + (inMatrix.matrix[3][2]);
        float     w = (inVector.x * inMatrix.matrix[0][3]) + (inVector.y * inMatrix.matrix[1][3]) + (inVector.z * inMatrix.matrix[2][3]) + (inMatrix.matrix[3][3]);

        if (w != 0.0f)          // Turning back to 3 Dimensions.
        {
            outVector.x /= w;
            outVector.y /= w;
            outVector.z /= w;
        }
    }
    

public:
    Engine3D()
    {
        m_sAppName = L"3D Demo";
    }


    bool OnUserCreate() override 
    {
        // Creating a cube mesh --------------------------------------------------------------
        meshCube.vTriangle = {
            // South face
            { (0.0f, 0.0f, 0.0f)   ,   (0.0f, 1.0f, 0.0f)   ,   (1.0f, 1.0f, 0.0f) },
            { (0.0f, 0.0f, 0.0f)   ,   (1.0f, 1.0f, 0.0f)   ,   (1.0f, 0.0f, 0.0f) },

            // East face
            { (1.0f, 0.0f, 0.0f)   ,   (1.0f, 1.0f, 0.0f)   ,   (1.0f, 1.0f, 1.0f) },
            { (1.0f, 0.0f, 0.0f)   ,   (1.0f, 1.0f, 1.0f)   ,   (1.0f, 0.0f, 1.0f) },

            // North face
            { (1.0f, 0.0f, 1.0f)   ,   (1.0f, 1.0f, 1.0f)   ,   (0.0f, 1.0f, 1.0f) },
            { (1.0f, 0.0f, 1.0f)   ,   (0.0f, 1.0f, 1.0f)   ,   (0.0f, 0.0f, 1.0f) },

            // West face
            { (0.0f, 0.0f, 1.0f)   ,   (0.0f, 1.0f, 1.0f)   ,   (0.0f, 1.0f, 0.0f) },
            { (0.0f, 0.0f, 1.0f)   ,   (0.0f, 1.0f, 0.0f)   ,   (0.0f, 0.0f, 0.0f) },

            // Top face
            { (0.0f, 1.0f, 0.0f)   ,   (0.0f, 1.0f, 1.0f)   ,   (1.0f, 1.0f, 1.0f) },
            { (0.0f, 1.0f, 0.0f)   ,   (1.0f, 1.0f, 1.0f)   ,   (1.0f, 1.0f, 0.0f) },

            // Bottom face
            { (1.0f, 0.0f, 1.0f)   ,   (0.0f, 0.0f, 1.0f)   ,   (0.0f, 0.0f, 0.0f) },
            { (1.0f, 0.0f, 1.0f)   ,   (0.0f, 0.0f, 0.0f)   ,   (1.0f, 0.0f, 0.0f) }
        };
        

        // Setting up the data for the Projection matrix. NOTE: (migth make it "const" later)
        float fNear        = 0.1f;                                                  // User face distance to the screen
        float fFar         = 1000.0f;                                               // Greatest distance of the Z axis able to be seen
        float fFov         = 90.0f;                                                 // Theta angle that defines the Field of view
        float fAspectRatio = (float)ScreenHeight() / (float)ScreenWidth();          // Screen aspect ratio based on the console window size
        float fFovScaleRad = 1.0f / std::tanf(fFov * 0.5f / 180.0f * 3.14159f);     // FOV scaling factor in radians NOTE: (3.14159f can be abstracted as PI latter)


        // Populating the Projection matrix --------------------------------------------------
        matProjection.matrix[0][0] = fAspectRatio * fFovScaleRad;
        matProjection.matrix[1][1] = fFovScaleRad;
        matProjection.matrix[2][2] = fFar / (fFar - fNear);
        matProjection.matrix[3][2] = (-fFar * fNear) / (fFar - fNear);
        matProjection.matrix[2][3] = 1.0f;
        matProjection.matrix[3][3] = 0.0f;



        return true; 
    }
    
    bool OnUserUpdate(float fElapsedTime) override
    { 
        Fill(0, 0, m_nScreenWidth, m_nScreenHeight, PIXEL_SOLID, FG_BLACK);
        
        for (Triangle& tri : meshCube.vTriangle)
        {
            Triangle triProjected;
            MultiplyMatrixVector(tri.vertex[0], triProjected.vertex[0], matProjection);
            MultiplyMatrixVector(tri.vertex[1], triProjected.vertex[1], matProjection);
            MultiplyMatrixVector(tri.vertex[2], triProjected.vertex[2], matProjection);

            // Scale into console view
            triProjected.vertex[0].x += 1.0f; triProjected.vertex[0].y += 1.0f;
            triProjected.vertex[1].x += 1.0f; triProjected.vertex[1].y += 1.0f;
            triProjected.vertex[2].x += 1.0f; triProjected.vertex[2].y += 1.0f;

            triProjected.vertex[0].x *= 0.5f * (float)ScreenWidth ();
            triProjected.vertex[0].y *= 0.5f * (float)ScreenHeight();

            triProjected.vertex[1].x *= 0.5f * (float)ScreenWidth ();
            triProjected.vertex[1].y *= 0.5f * (float)ScreenHeight();
            
            triProjected.vertex[2].x *= 0.5f * (float)ScreenWidth ();
            triProjected.vertex[2].y *= 0.5f * (float)ScreenHeight();


            DrawTriangle(triProjected.vertex[0].x, triProjected.vertex[0].y, 
                         triProjected.vertex[1].x, triProjected.vertex[1].y, 
                         triProjected.vertex[2].x, triProjected.vertex[2].y,
                         PIXEL_SOLID, FG_WHITE);
        }
        
        return true; 
    }
};



int main()
{
    Engine3D demo;
    if (demo.ConstructConsole(256, 240, 4, 4))
        demo.Start();
    else
        return -1;
}