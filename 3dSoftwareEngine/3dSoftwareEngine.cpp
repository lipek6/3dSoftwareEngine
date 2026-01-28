#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

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
class Engine3D : public olc::PixelGameEngine
{
private:
    
    float fTheta;
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
        sAppName = "3D Demo";
    }


    bool OnUserCreate() override 
    {
        // Creating a cube mesh --------------------------------------------------------------
        meshCube.vTriangle = {
            // South face
            { 0.0f, 0.0f, 0.0f   ,   0.0f, 1.0f, 0.0f   ,   1.0f, 1.0f, 0.0f },
            { 0.0f, 0.0f, 0.0f   ,   1.0f, 1.0f, 0.0f   ,   1.0f, 0.0f, 0.0f },

            // East face
            { 1.0f, 0.0f, 0.0f   ,   1.0f, 1.0f, 0.0f   ,   1.0f, 1.0f, 1.0f },
            { 1.0f, 0.0f, 0.0f   ,   1.0f, 1.0f, 1.0f   ,   1.0f, 0.0f, 1.0f },

            // North face
            { 1.0f, 0.0f, 1.0f   ,   1.0f, 1.0f, 1.0f   ,   0.0f, 1.0f, 1.0f },
            { 1.0f, 0.0f, 1.0f   ,   0.0f, 1.0f, 1.0f   ,   0.0f, 0.0f, 1.0f },

            // West face
            { 0.0f, 0.0f, 1.0f   ,   0.0f, 1.0f, 1.0f   ,   0.0f, 1.0f, 0.0f },
            { 0.0f, 0.0f, 1.0f   ,   0.0f, 1.0f, 0.0f   ,   0.0f, 0.0f, 0.0f },

            // Top face
            { 0.0f, 1.0f, 0.0f   ,   0.0f, 1.0f, 1.0f   ,   1.0f, 1.0f, 1.0f },
            { 0.0f, 1.0f, 0.0f   ,   1.0f, 1.0f, 1.0f   ,   1.0f, 1.0f, 0.0f },

            // Bottom face
            { 1.0f, 0.0f, 1.0f   ,   0.0f, 0.0f, 1.0f   ,   0.0f, 0.0f, 0.0f },
            { 1.0f, 0.0f, 1.0f   ,   0.0f, 0.0f, 0.0f   ,   1.0f, 0.0f, 0.0f }
        };
        

        // Setting up the data for the Projection matrix. NOTE: (migth make it "const" later)
        float fNear        = 0.1f;                                                  // User face distance to the screen
        float fFar         = 1000.0f;                                               // Greatest distance of the Z axis able to be seen
        float fFov         = 90.0f;                                                 // Theta angle that defines the Field of view
        float fAspectRatio = (float)ScreenHeight() / (float)ScreenWidth();          // Screen aspect ratio based on the window size
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
        Clear(olc::BLACK);
      
        Matrix4x4 matRotateZ, matRotateX;
        
        fTheta += 1.0f * fElapsedTime;

        // Rotation Z
        matRotateZ.matrix[0][0] =  cosf(fTheta);
        matRotateZ.matrix[0][1] =  sinf(fTheta);
        matRotateZ.matrix[1][0] = -sinf(fTheta);
        matRotateZ.matrix[1][1] =  cosf(fTheta);
        matRotateZ.matrix[2][2] =  1; 
        matRotateZ.matrix[3][3] =  1;

        // Rotation X
        matRotateX.matrix[0][0] =  1;
        matRotateX.matrix[1][1] =  cosf(fTheta * 0.5f);
        matRotateX.matrix[1][2] =  sinf(fTheta * 0.5f);
        matRotateX.matrix[2][1] = -sinf(fTheta * 0.5f);
        matRotateX.matrix[2][2] =  cosf(fTheta * 0.5f);
        matRotateX.matrix[3][3] =  1;


        for (Triangle& tri : meshCube.vTriangle)
        {

            // Rotate the object
            Triangle triRotatedZ;
            MultiplyMatrixVector(tri.vertex[0], triRotatedZ.vertex[0], matRotateZ);
            MultiplyMatrixVector(tri.vertex[1], triRotatedZ.vertex[1], matRotateZ);
            MultiplyMatrixVector(tri.vertex[2], triRotatedZ.vertex[2], matRotateZ);
            Triangle triRotatedZX;
            MultiplyMatrixVector(triRotatedZ.vertex[0], triRotatedZX.vertex[0], matRotateX);
            MultiplyMatrixVector(triRotatedZ.vertex[1], triRotatedZX.vertex[1], matRotateX);
            MultiplyMatrixVector(triRotatedZ.vertex[2], triRotatedZX.vertex[2], matRotateX);


            // Translate the triangles on the Z axis to remove the camera from its inside
            Triangle triTranslated = triRotatedZX;
            triTranslated.vertex[0].z = triRotatedZX.vertex[0].z + 3.0f;
            triTranslated.vertex[1].z = triRotatedZX.vertex[1].z + 3.0f;
            triTranslated.vertex[2].z = triRotatedZX.vertex[2].z + 3.0f;

            
            // Run all vertexis of the translated and rotated triangles on the projection matrix
            Triangle triProjected;
            MultiplyMatrixVector(triTranslated.vertex[0], triProjected.vertex[0], matProjection);
            MultiplyMatrixVector(triTranslated.vertex[1], triProjected.vertex[1], matProjection);
            MultiplyMatrixVector(triTranslated.vertex[2], triProjected.vertex[2], matProjection);


            // Scale the result into the console view
            float offset = 1.0f;
            triProjected.vertex[0].x += offset; triProjected.vertex[0].y += offset;
            triProjected.vertex[1].x += offset; triProjected.vertex[1].y += offset;
            triProjected.vertex[2].x += offset; triProjected.vertex[2].y += offset;

            triProjected.vertex[0].x *= 0.5f * (float)ScreenWidth ();
            triProjected.vertex[0].y *= 0.5f * (float)ScreenHeight();

            triProjected.vertex[1].x *= 0.5f * (float)ScreenWidth ();
            triProjected.vertex[1].y *= 0.5f * (float)ScreenHeight();
            
            triProjected.vertex[2].x *= 0.5f * (float)ScreenWidth ();
            triProjected.vertex[2].y *= 0.5f * (float)ScreenHeight();


            // Draw the triangles to the screen
            DrawTriangle(triProjected.vertex[0].x, triProjected.vertex[0].y, 
                         triProjected.vertex[1].x, triProjected.vertex[1].y, 
                         triProjected.vertex[2].x, triProjected.vertex[2].y,
                         olc::WHITE);
        }
        
        return true; 
    }
};



int main()
{
    Engine3D demo;
    if (demo.Construct(256, 240, 4, 4))
        demo.Start();
    else
        return -1;
}