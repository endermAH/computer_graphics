// CG_Lab_1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "stdafx.h"
#include <iostream>

#include "Framework.h"
#include "../EDUC/RenderRotatingCubes.h"
#include "../EDUC/RenderCube.h"
#include "../EDUC/RenderTriangle.h"
#include "../Pong/RenderPaddle.h"
#include "GameSystem/Game.h"
#include "../Pong/Paddle.h"
#include "../Pong/Ball.h"
#include "../Pong/Pong.h"

/*void test_framework() {
    Framework framework;

    //RenderTriangle *render = new RenderTriangle();
    //RenderCube *render = new RenderCube();
    RenderRotatingCubes *render = new RenderRotatingCubes();
    
    FrameworkDescriptor framework_desc;
    framework_desc.window_descriptor.width = 640;
    framework_desc.window_descriptor.height = 480;
    framework_desc.render = render;
    
    framework.Init(framework_desc);
    
    //framework.AddInputListener(controller);

    framework.Run();

    framework.Close();
}*/

void test_pong() {
    Pong *pong = new Pong(640, 480);
    pong->GameStart();
}

int main()
{
    test_pong();
    return 0;
}