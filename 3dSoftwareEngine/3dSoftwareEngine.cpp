#include "olcConsoleGameEngine.h"



class Engine3D : public olcConsoleGameEngine
{
public:
    Engine3D()
    {
        m_sAppName = L"3D Demo";
    }

    bool OnUserCreate() override 
    {
        return true; 
    }
    
    bool OnUserUpdate(float fElapsedTime) override
    { 
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

    std::cout << "Hello World!\n";
}