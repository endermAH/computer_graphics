#include "InputManager.h"
#include "../Log.h"
#include "InputCodes.h"
#include "InputListener.h"

void InputManager::Init()
{
	mouse_wheel_ = current_x_ = current_y_ = 0;
	Log::LogDebug("Input Manager initialized");
}

void InputManager::Close()
{
	if (!listener_list_.empty())
		listener_list_.clear();
	Log::LogDebug("Input Manager closed");
}

void InputManager::SetWinRect(const RECT &winrect)
{
	window_rect_.left = winrect.left;
	window_rect_.right = winrect.right;
	window_rect_.top = winrect.top;
	window_rect_.bottom = winrect.bottom;
}

void InputManager::AddListener(InputListener *Listener)
{
	listener_list_.push_back(Listener);
}

void InputManager::Run(const UINT &msg, WPARAM wParam, LPARAM lParam)
{
	if (listener_list_.empty())
		return;

	eKeyCodes KeyIndex;
	wchar_t buffer[1];
	BYTE lpKeyState[256];

	EventCursor();// событие движения мыши
	switch(msg)
	{
	case WM_KEYDOWN:
		KeyIndex = static_cast<eKeyCodes>(wParam);
		GetKeyboardState(lpKeyState);
		ToUnicode(wParam, HIWORD(lParam)&0xFF, lpKeyState, buffer, 1, 0);
		EventKey(KeyIndex,buffer[0],true);
		break;
	case WM_KEYUP:
		KeyIndex = static_cast<eKeyCodes>(wParam);
		GetKeyboardState(lpKeyState);
		ToUnicode(wParam, HIWORD(lParam)&0xFF, lpKeyState, buffer, 1, 0);
		EventKey(KeyIndex,buffer[0],false);
		break;
	case WM_LBUTTONDOWN:
		EventMouse(MOUSE_LEFT,true);
		break;
	case WM_LBUTTONUP:
		EventMouse(MOUSE_LEFT,false);
		break;
	case WM_RBUTTONDOWN:
		EventMouse(MOUSE_RIGHT,true);
		break;
	case WM_RBUTTONUP:
		EventMouse(MOUSE_RIGHT,false);
		break;
	case WM_MBUTTONDOWN:
		EventMouse(MOUSE_MIDDLE,true);
		break;
	case WM_MBUTTONUP:
		EventMouse(MOUSE_MIDDLE,false);
		break;
	case WM_MOUSEWHEEL:
		EventMouseWheel( (short)GET_WHEEL_DELTA_WPARAM(wParam) / WHEEL_DELTA );
		break;
	}
}

void InputManager::EventCursor()
{
	POINT Position;
	GetCursorPos(&Position);	// получаем текущую позицию курсора

	Position.x -= window_rect_.left;
	Position.y -= window_rect_.top;

	if (current_x_==Position.x && current_y_==Position.y)
		return;

	current_x_ = Position.x;
	current_y_ = Position.y;

	for (auto listener : listener_list_) {
		if (listener->MouseMove(MouseEvent(current_x_, current_y_))) return;
	}
}

void InputManager::EventMouse(const eMouseKeyCodes Code, bool press)
{
	for(auto listener : listener_list_)
	{
		if (press==true)
		{
			if (listener->MousePressed(MouseEventClick(Code, current_x_,current_y_))==true)
				return;
		}
		else
		{
			if (listener->MouseReleased(MouseEventClick(Code, current_x_,current_y_))==true)
				return;
		}
	}
}

void InputManager::EventMouseWheel(short Value)
{
	if (mouse_wheel_==Value)
		return;

	mouse_wheel_ = Value;

	for(auto listener : listener_list_)
	{
		if (listener->MouseWheel(MouseEventWheel(mouse_wheel_, current_x_,current_y_))==true)
			return;
	}
}

void InputManager::EventKey(const eKeyCodes KeyCode, const wchar_t ch, bool press)
{
	for(auto listener : listener_list_)
	{
		if (!listener)
			continue;
		if (press==true)
		{
			if (listener->KeyPressed(KeyEvent(ch,KeyCode))==true)
				return;
		}
		else
		{
			if (listener->KeyReleased(KeyEvent(ch,KeyCode))==true)
				return;
		}
	}
}