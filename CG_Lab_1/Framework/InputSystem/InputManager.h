#pragma once

#include <Windows.h>
#include <list>

#include "InputCodes.h"
#include "InputListener.h"

class InputManager
{
public:
	void Init();
	void Close();
	void Run(const UINT &msg, WPARAM wParam, LPARAM lParam);
	void AddListener(InputListener *Listener);
	void SetWinRect(const RECT &winrect);
	
private:
	std::list<InputListener*> listener_list_;

	RECT window_rect_;
	int current_x_;
	int current_y_;
	int mouse_wheel_;
	
private:
	void EventCursor();
	void EventMouse(const eMouseKeyCodes KeyCode, bool press);
	void EventMouseWheel(short Value);
	void EventKey(const eKeyCodes KeyCode, const wchar_t ch, bool press);
};
