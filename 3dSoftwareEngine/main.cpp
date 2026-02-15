/*
    Hello! This is a 3D software engine, that means that we will run 3D graphics on the CPU :).
    This project is the way I found to start on graphics and game programming, because "If you
    want to learn about wheels, there is nothing wrong on reinventing them".

    All of this only exists because of the amazing lectures of Javidx9 / OneLoneCoder. Thanks mate, you made me brave enough to
    get into low level game programming and 3D graphics programming. For anyone reading this, please take a look at his channel:
    https://www.youtube.com/@javidx9 The "Code-it-yourself" series is an incredible learning resource.
*/


#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"
#include "Engine3d.h"

int main()
{
    Engine3d game;
    //if (game.Construct(256, 240, 4, 4))
    //    game.Start();
    //else
    //    return -1;

    // HIGH DEF VERSION
    if (game.Construct(1920, 1080, 1, 1))
      game.Start();
    else
      return -1;
}