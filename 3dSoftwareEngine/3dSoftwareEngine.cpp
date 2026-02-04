/*  
    Hello! This is a 3D software engine, that means that we will run 3D graphics on the CPU :).
    This project is the way I found to start on graphics and game programming, because "If you 
    want to learn about wheels, there is nothing wrong on reinventing them".

    All of this only exists because of the amazing lectures of Javidx9 / OneLoneCoder. Thanks mate, you made me brave enough to
    get into low level game programming and 3D graphics programming. For anyone reading this, please take a look at his channel:
    https://www.youtube.com/@javidx9 The "Code-it-yourself" series is an incredible learning resource.
*/





// HEADERS ===================================================================================

#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"
#include <fstream>
#include <sstream>
#include <algorithm>





// MACROS ==================================================================================== 

#define MAX_INLINE_CHARS_OBJFILE 128





// STRUCTS ===================================================================================

// 3 Dimensional vector.
struct Vec3d
{
    float x = 0;
    float y = 0;
    float z = 0;
    float w = 1;
};



// Set of 3 Dimensional vectors used to represent triangles.
struct Triangle
{
    Vec3d vertex[3];

    olc::Pixel color;
};



// Set of triangles used to Represent the objects.
struct Mesh             
{
    std::vector<Triangle> vTriangle;

    // Load a .obj type of file into a mesh
    bool LoadFromObjectFile(std::string sFilename)
    {
        std::ifstream file(sFilename);
        if (!file.is_open())
            return false;


        std::vector<Vec3d> vVertex;     // Pool to store the vertecis data
        while (!file.eof())
        {
            char line[MAX_INLINE_CHARS_OBJFILE];
            file.getline(line, MAX_INLINE_CHARS_OBJFILE);
            
            std::stringstream lineStream; lineStream << line;
    
            if (line[0] == 'v')
            {
                char dataType;
                Vec3d vertex;

                lineStream >> dataType >> vertex.x >> vertex.y >> vertex.z;
                vVertex.push_back(vertex);
            }
            else if (line[0] == 'f')
            {
                char dataType;
                size_t idx0, idx1, idx2;

                lineStream >> dataType >> idx0 >> idx1 >> idx2;
                vTriangle.push_back({ vVertex[idx0 - 1], vVertex[idx1 - 1], vVertex[idx2 - 1] });
            }
        }


        file.close();       // NOTE: Closing this migth cause problems later. Also, when out of scope, this shit closes itself, so this migth be unecessary.
        return true;
    }
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
    Mesh meshMech;
    Matrix4x4 matProjection;
    Vec3d vCamera;
    float fTheta = 0;




    // Utility Functions ---------------------------------------------------------------------
    
    // Multiply a matrix by a vector and returns a new vector
    Vec3d Matrix_MultiplyVector(Matrix4x4 &inMatrix, Vec3d &inVector)
    {
        Vec3d outVector;
        outVector.x = inVector.x * inMatrix.matrix[0][0] + inVector.y * inMatrix.matrix[1][0] + inVector.z * inMatrix.matrix[2][0] + inVector.w * inMatrix.matrix[3][0];
        outVector.y = inVector.x * inMatrix.matrix[0][1] + inVector.y * inMatrix.matrix[1][1] + inVector.z * inMatrix.matrix[2][1] + inVector.w * inMatrix.matrix[3][1];
        outVector.z = inVector.x * inMatrix.matrix[0][2] + inVector.y * inMatrix.matrix[1][2] + inVector.z * inMatrix.matrix[2][2] + inVector.w * inMatrix.matrix[3][2];
        outVector.w = inVector.x * inMatrix.matrix[0][3] + inVector.y * inMatrix.matrix[1][3] + inVector.z * inMatrix.matrix[2][3] + inVector.w * inMatrix.matrix[3][3];
        return outVector;
    }



    // Creates an identity matrix (that means that we are gonna create a matrix that doesn't does anything)
    Matrix4x4 Matrix_MakeIdentity()
    {
        Matrix4x4 identityMatrix;
        identityMatrix.matrix[0][0] = 1.0f;
        identityMatrix.matrix[1][1] = 1.0f;
        identityMatrix.matrix[2][2] = 1.0f;
        identityMatrix.matrix[3][3] = 1.0f;
        return identityMatrix;
    }
    


    // Creates a matrix that rotates a vector on the X axis using a given angle in radians
    Matrix4x4 Matrix_MakeRotationX(float fAngleRad)
    {
        Matrix4x4 matrixRotX;
        matrixRotX.matrix[0][0] =  1.0f;
        matrixRotX.matrix[1][1] =  cosf(fAngleRad);
        matrixRotX.matrix[1][2] =  sinf(fAngleRad);
        matrixRotX.matrix[2][1] = -sinf(fAngleRad);
        matrixRotX.matrix[2][2] =  cosf(fAngleRad);
        matrixRotX.matrix[3][3] =  1.0f;
        return matrixRotX;
    }



    // Creates a matrix that rotates a vector on the Y axis using a given angle in radians
    Matrix4x4 Matrix_MakeRotationY(float fAngleRad)
    {
        Matrix4x4 matrixRotY;
        matrixRotY.matrix[0][0] =  cosf(fAngleRad);
        matrixRotY.matrix[0][2] =  sinf(fAngleRad);
        matrixRotY.matrix[2][0] = -sinf(fAngleRad);
        matrixRotY.matrix[1][1] =  1.0f;
        matrixRotY.matrix[2][2] =  cosf(fAngleRad);
        matrixRotY.matrix[3][3] =  1.0f;
        return matrixRotY;
    }



    // Creates a matrix that rotates a vector on the Z axis using a given angle in radians
    Matrix4x4 Matrix_MakeRotationZ(float fAngleRad)
    {
        Matrix4x4 matrixRotZ;
        matrixRotZ.matrix[0][0] =  cosf(fAngleRad);
        matrixRotZ.matrix[0][1] =  sinf(fAngleRad);
        matrixRotZ.matrix[1][0] = -sinf(fAngleRad);
        matrixRotZ.matrix[1][1] =  cosf(fAngleRad);
        matrixRotZ.matrix[2][2] =  1.0f;
        matrixRotZ.matrix[3][3] =  1.0f;
        return matrixRotZ;
    }



    // Creates a matrix that translates a vector using given values for X, Y and Z
    Matrix4x4 Matrix_MakeTranslation(float x, float y, float z)
    {
        Matrix4x4 matrixTrans;
        matrixTrans.matrix[0][0] = 1.0f;
        matrixTrans.matrix[1][1] = 1.0f;
        matrixTrans.matrix[2][2] = 1.0f;
        matrixTrans.matrix[3][3] = 1.0f;
        matrixTrans.matrix[3][0] = x;
        matrixTrans.matrix[3][1] = y;
        matrixTrans.matrix[3][2] = z;
        return matrixTrans;
    }



    // Creates the Projection Matrix using the given Field of view in degrees, the screen aspect ratio (h/w), the distance from the user to the screen and the max Z distance
    Matrix4x4 Matrix_MakeProjection(float fFovDegrees, float fAspectRatio, float fNear, float fFar)
    {
        float fFovRad = 1.0f / tanf(fFovDegrees * 0.5f / 180.0f * 3.14159);
        Matrix4x4 matrixProjection;
        matrixProjection.matrix[0][0] = fAspectRatio * fFovRad;
        matrixProjection.matrix[1][1] = fFovRad;
        matrixProjection.matrix[2][2] = fFar / (fFar - fNear);
        matrixProjection.matrix[3][2] = (-fFar * fNear) / (fFar - fNear);
        matrixProjection.matrix[2][3] = 1.0f;
        matrixProjection.matrix[3][3] = 0.0f;
        return matrixProjection;
    }



    // Multiply two matrices, returning a new matrix
    Matrix4x4 Matrix_MultiplyMatrix(Matrix4x4 &inMatrix1, Matrix4x4 &inMatrix2)
    {
        Matrix4x4 outMatrix;
        for (int col = 0; col < 4; col++)
            for (int row = 0; row < 4; row++)
                outMatrix.matrix[row][col] = inMatrix1.matrix[row][0] * inMatrix2.matrix[0][col] + 
                                             inMatrix1.matrix[row][1] * inMatrix2.matrix[1][col] + 
                                             inMatrix1.matrix[row][2] * inMatrix2.matrix[2][col] + 
                                             inMatrix1.matrix[row][3] * inMatrix2.matrix[3][col];
        return outMatrix;
    }



    // NOTE: NO IDEA
    /*
    
    Matrix4x4 Matrix_PointAt(Vec3d& position, Vec3d& target, Vec3d& up)
    {
    
    }

    */


    // NOTE: NO IDEA
    /*
    
    Matrix4x4 Matrix_QuickInverse(Matrix4x4& inMatrix)
    {

    }

    */




    // Add two vectors together and returns a new vector
    Vec3d Vector_Add(Vec3d &inVector1, Vec3d &inVector2)
    {
        return { inVector1.x + inVector2.x, inVector1.y + inVector2.y, inVector1.z + inVector2.z };
    }



    // Subtract two vectors and returns a new vector (Subtracts the first from the second vector passed as input)
    Vec3d Vector_Sub(Vec3d &inVector1, Vec3d &inVector2)
    {
        return { inVector1.x - inVector2.x, inVector1.y - inVector2.y, inVector1.z - inVector2.z };
    }
    


    // Multiply a vector by a given scalar
    Vec3d Vector_Mul(Vec3d &inVector, float scalar)
    {
        return { inVector.x * scalar, inVector.y * scalar, inVector.z * scalar };
    }



    // Divide a vector by a given scalar
    Vec3d Vector_Div(Vec3d &inVector, float scalar)
    {
        return { inVector.x / scalar, inVector.y / scalar, inVector.z / scalar };
    }
    
    

    // Calculates the Dot product between two vectors, returning a not normalized value.
    float Vector_DotProduct(Vec3d &inVector1, Vec3d &inVector2)
    {
        return inVector1.x * inVector2.x + inVector1.y * inVector2.y + inVector1.z * inVector2.z;
    }



    // Returns the legth of a given vector by using pythagoras (dot product of the vector with itself does the same thing)
    float Vector_Length(Vec3d &inVector)
    {
        return sqrtf(Vector_DotProduct(inVector, inVector));
    }



    // Normalise a vector by dividing all the vector dimensions by its lenght
    Vec3d Vector_Normalise(Vec3d &inVector)
    {
        float lenght = Vector_Length(inVector);
        return { inVector.x / lenght, inVector.y / lenght, inVector.z / lenght };
    }
    


    // Calculates the cross product between two given vectors, returning a new vector
    Vec3d Vector_CrossProduct(Vec3d &inVector1, Vec3d& inVector2)
    {
        Vec3d outVector;
        outVector.x = inVector1.y * inVector2.z - inVector1.z * inVector2.y;
        outVector.y = inVector1.z * inVector2.x - inVector1.x * inVector2.z;
        outVector.z = inVector1.x * inVector2.y - inVector1.y * inVector2.z;
        return outVector;
    }

    
    // 	vec3d Vector_IntersectPlane(vec3d &plane_p, vec3d &plane_n, vec3d &lineStart, vec3d &lineEnd)
    // 	int Triangle_ClipAgainstPlane(vec3d plane_p, vec3d plane_n, triangle &in_tri, triangle &out_tri1, triangle &out_tri2)
    
    
    
    // Input parameter lum must be between 0 and 1 - i.e. [0, 1] (normalized)
    olc::Pixel GetColor(float lum)
    {
        int nValue = (int)(std::max(lum, 0.20f) * 255.0f);
        return olc::Pixel(nValue, nValue, nValue);
    }
    




public:
    Engine3D()
    {
        sAppName = "3D Demo";
    }




    // GAME SETUP ============================================================================
    bool OnUserCreate() override 
    {
        
        if (meshMech.LoadFromObjectFile("Mech01_Official01.obj"))                                                           // Loading cool ass mech 
            std::cout << "'.obj file' Loaded Sucessfully" << std::endl;
        else
            std::cout << "Error loading '.obj file' - Unable to open it" << std::endl;

        matProjection = Matrix_MakeProjection(90.0f, (float)ScreenHeight() / (float)ScreenWidth(), 0.1f, 1000.0f);        // Sets up the projection matrix

        return true; 
    }
    




    // GAME LOOP =============================================================================
    bool OnUserUpdate(float fElapsedTime) override  
    { 
        // Setup
        Clear(olc::BLACK);
        fTheta += 1.0f * fElapsedTime;

        Matrix4x4 matRotateX   = Matrix_MakeRotationX  (fTheta);
        Matrix4x4 matRotateZ   = Matrix_MakeRotationZ  (fTheta);                                                           
        Matrix4x4 matTranslate = Matrix_MakeTranslation(0.0f, 0.0f, 80.0f);

        Matrix4x4 matWorld = Matrix_MakeIdentity();
                  matWorld = Matrix_MultiplyMatrix(matRotateZ, matRotateX);
                  matWorld = Matrix_MultiplyMatrix(matWorld, matTranslate);




        //  Store triangles for later rasterization
        std::vector<Triangle> vTrianglesToRasterize;
        for (Triangle& tri : meshMech.vTriangle)
        {
            Triangle triTransformed, triProjected;

            
            // Put triangles on their place (transform them)
            triTransformed.vertex[0] = Matrix_MultiplyVector(matWorld, tri.vertex[0]);
            triTransformed.vertex[1] = Matrix_MultiplyVector(matWorld, tri.vertex[1]);
            triTransformed.vertex[2] = Matrix_MultiplyVector(matWorld, tri.vertex[2]);

            // Find the normals and the faces that are visible
            Vec3d line1  = Vector_Sub(triTransformed.vertex[1], triTransformed.vertex[0]);
            Vec3d line2  = Vector_Sub(triTransformed.vertex[2], triTransformed.vertex[0]);
            Vec3d normal = Vector_CrossProduct(line1, line2);
            normal = Vector_Normalise(normal);



            Vec3d vCameraRay = Vector_Sub(triTransformed.vertex[0], vCamera);

            // If the triangle is visible, draw it
            if (Vector_DotProduct(normal, vCameraRay) < 0.0f) 
            {
                // Illuminate the scene
                Vec3d ligthDirection = { 0.0f, 1.0f, -1.0f };
                ligthDirection = Vector_Normalise(ligthDirection);
            

                // Check the ligth alignement with the surface
                float dpLigthNormal = std::max(0.1f, Vector_DotProduct(ligthDirection, normal));


                // Choose the color of the triangle
                triTransformed.color = GetColor(dpLigthNormal);


                // Run transformed triangles on the projection matrix (3D --> 2D)
                Triangle triProjected;

                triProjected.vertex[0] = Matrix_MultiplyVector(matProjection, triTransformed.vertex[0]);
                triProjected.vertex[1] = Matrix_MultiplyVector(matProjection, triTransformed.vertex[1]);
                triProjected.vertex[2] = Matrix_MultiplyVector(matProjection, triTransformed.vertex[2]);
                

                // Normalise vectors
                triProjected.vertex[0] = Vector_Div(triProjected.vertex[0], triProjected.vertex[0].w);
                triProjected.vertex[1] = Vector_Div(triProjected.vertex[1], triProjected.vertex[1].w);
                triProjected.vertex[2] = Vector_Div(triProjected.vertex[2], triProjected.vertex[2].w);


                // Pass the color
                triProjected.color = triTransformed.color;



                // Scale the result into the console view
                Vec3d vOffsetView = { 1,1,0 };

                triProjected.vertex[0] = Vector_Add(triProjected.vertex[0], vOffsetView);
                triProjected.vertex[1] = Vector_Add(triProjected.vertex[1], vOffsetView);
                triProjected.vertex[2] = Vector_Add(triProjected.vertex[2], vOffsetView);

                triProjected.vertex[0].x *= 0.5f * (float)ScreenWidth();
                triProjected.vertex[0].y *= 0.5f * (float)ScreenHeight();

                triProjected.vertex[1].x *= 0.5f * (float)ScreenWidth();
                triProjected.vertex[1].y *= 0.5f * (float)ScreenHeight();

                triProjected.vertex[2].x *= 0.5f * (float)ScreenWidth();
                triProjected.vertex[2].y *= 0.5f * (float)ScreenHeight();



                // Store Triangles for sorting
                vTrianglesToRasterize.push_back(triProjected);
            }
        }




        // Sort in ascending order 
        std::sort(vTrianglesToRasterize.begin(), vTrianglesToRasterize.end(), [](const Triangle &triangle0, const Triangle &triangle1)
        {
            float midPointZ0 = (triangle0.vertex[0].z + triangle0.vertex[1].z + triangle0.vertex[2].z) / 3.0f;
            float midPointZ1 = (triangle1.vertex[0].z + triangle1.vertex[1].z + triangle1.vertex[2].z) / 3.0f;

            return midPointZ0 > midPointZ1;         // If Z0 is greater than Z1, swap them in the vector
        });
        



        // Draw/Rasterize the triangles to the screen
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
};





int main()
{
    Engine3D demo;
    //if (demo.Construct(256, 240, 4, 4))
    //    demo.Start();
    //else
    //    return -1;

    // HIGH DEF VERSION
    if (demo.Construct(1920, 1080, 1, 1))
      demo.Start();
    else
      return -1;
}




/*        // Creating a cube mesh (CLOCKWISE) --------------------------------------------------
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
*/
