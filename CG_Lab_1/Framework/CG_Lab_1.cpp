// CG_Lab_1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>

#include "Log.h"
#include "Framework.h"
#include "InputSystem/InputListener.h"
#include "Render.h"
#include "RenderTriangle.h"


int main()
{
    Framework framework;

    RenderTriangle *render = new RenderTriangle();

    framework.SetRender(render);
    framework.Init();
    //framework.AddInputListener(controller);

    framework.Run();

    framework.Close();

    return 0;
}