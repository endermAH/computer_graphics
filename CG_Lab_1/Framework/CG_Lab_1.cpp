// CG_Lab_1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "stdafx.h"
#include <iostream>

#include "Framework.h"
#include "RenderCube.h"
#include "RenderTriangle.h"


int main()
{
    Framework framework;

    //RenderTriangle *render = new RenderTriangle();
    RenderCube *render = new RenderCube();
    
    FrameworkDescriptor framework_desc;
    framework_desc.window_descriptor.width = 640;
    framework_desc.window_descriptor.height = 480;
    framework_desc.render = render;
    
    framework.Init(framework_desc);
    
    //framework.AddInputListener(controller);

    framework.Run();

    framework.Close();

    return 0;
}