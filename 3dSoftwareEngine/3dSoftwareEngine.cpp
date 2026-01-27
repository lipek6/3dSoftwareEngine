#include "olcConsoleGameEngine.h"

// STRUCTS ===================================================================================

// 3 Dimensional vector 
struct vec3d
{
    float x, y, z;
};


// Set of 3 Dimensional vectors used to represent triangles
struct triangle
{
    vec3d point[3];
};


// Set of triangles used to Represent the objects.
struct mesh             
{
    std::vector<triangle> vTriangle;
};


// 
struct matrix4x4
{

};




// GAME ENGINE CLASS =========================================================================
class Engine3D : public olcConsoleGameEngine
{
private:
    mesh meshCube;

public:
    Engine3D()
    {
        m_sAppName = L"3D Demo";
    }

    bool OnUserCreate() override 
    {
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
        
        
        return true; 
    }
    
    bool OnUserUpdate(float fElapsedTime) override
    { 
        Fill(0, 0, m_nScreenWidth, m_nScreenHeight, PIXEL_SOLID, FG_BLACK);
        
        for (const triangle& tri : meshCube.vTriangle)
        {

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