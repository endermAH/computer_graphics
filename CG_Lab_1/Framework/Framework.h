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

	void SetRender(Render *render){m_render = render;}
	void AddInputListener(InputListener *listener);
protected:	
	bool m_frame();	

	Window *m_wnd;
	Render *m_render;
	InputManager *m_input;
	Log m_log;
	bool m_init;
};
