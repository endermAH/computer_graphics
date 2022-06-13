#pragma once

#include "stdafx.h"
#include "Window.h"
#include "Render.h"
#include "InputSystem/InputManager.h"
#include "InputSystem/InputControllers.h"

struct FrameworkDescriptor
{
	WindowDescriptor window_descriptor;
	Render *render;
};

class Framework
{
public:
	KeyboardController* Keyboard;
public:
	Framework();
	~Framework();

	bool Init(const FrameworkDescriptor &desc);
	void Run();
	void Close();
	
	void AddInputListener(InputListener *listener);
protected:
	Window *window_;
	Render *render_;
	InputManager *input_;
	bool init_;
protected:	
	bool frame_();	
};
