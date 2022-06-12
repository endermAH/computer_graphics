// CG_Lab_1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>

#include "Log.h"
#include "Framework.h"
#include "InputSystem/InputListener.h"
#include "Render.h"

class MyInput : public InputListener
{
public:
    bool KeyPressed(const KeyEvent &arg)
    { 
        Log::LogInfo("key press " + std::to_string(char(arg.wc)));
        return false; 
    }
    bool MouseMove(const MouseEvent &arg)
    {
        printf("mouse %d - %d\n",arg.x,arg.y);
        return false; 
    }
};

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
    MyInput *input = new MyInput();

    framework.SetRender(render);
    framework.Init();
    framework.AddInputListener(input);

    framework.Run();

    framework.Close();

    return 0;
}