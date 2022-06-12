#pragma once

#include "Window.h"
#include "Render.h"
#include "InputSystem/InputManager.h"
#include "InputSystem/InputControllers.h"
#include "Log.h"

class Framework
{
public:
	KeyboardController* Keyboard;
public:
	Framework();
	~Framework();

	bool Init();
	void Run();
	void Close();

	void SetRender(Render *render){render_ = render;}
	void AddInputListener(InputListener *listener);
protected:
	Window *window_;
	Render *render_;
	InputManager *input_;
	bool init_;
protected:	
	bool frame_();	
};
