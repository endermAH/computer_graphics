// CG_Lab_1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>

#include "Log.h"
#include "Framework.h"
#include "InputSystem/InputListener.h"
#include "Render.h"

class MyRender : public Render
{
public:
    bool Init(HWND nwnd){return true;}
    bool Draw(){return true;}
    void Close(){}
};

int main()
{
    Framework framework;

    MyRender *render = new MyRender();

    framework.SetRender(render);
    framework.Init();
    //framework.AddInputListener(controller);

    framework.Run();

    framework.Close();

    return 0;
}