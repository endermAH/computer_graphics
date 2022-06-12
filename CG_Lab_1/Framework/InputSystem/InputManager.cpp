#include "InputManager.h"
#include "../Log.h"
#include "InputCodes.h"
#include "InputListener.h"

void InputManager::Init()
{
	m_MouseWheel = m_curx = m_cury = 0;
	Log::LogDebug("Input Manager initialized");
}

void InputManager::Close()
{
	if (!m_Listener.empty())
		m_Listener.clear();
	Log::LogDebug("Input Manager closed");
}

void InputManager::SetWinRect(const RECT &winrect)
{
	m_windowrect.left = winrect.left;
	m_windowrect.right = winrect.right;
	m_windowrect.top = winrect.top;
	m_windowrect.bottom = winrect.bottom;
}

void InputManager::AddListener(InputListener *Listener)
{
	m_Listener.push_back(Listener);
}

void InputManager::Run(const UINT &msg, WPARAM wParam, LPARAM lParam)
{
	if (m_Listener.empty())
		return;

	eKeyCodes KeyIndex;
	wchar_t buffer[1];
	BYTE lpKeyState[256];

	m_eventcursor();// событие движения мыши
	switch(msg)
	{
	case WM_KEYDOWN:
		KeyIndex = static_cast<eKeyCodes>(wParam);
		GetKeyboardState(lpKeyState);
		ToUnicode(wParam, HIWORD(lParam)&0xFF, lpKeyState, buffer, 1, 0);
		m_eventkey(KeyIndex,buffer[0],true);
		break;
	case WM_KEYUP:
		KeyIndex = static_cast<eKeyCodes>(wParam);
		GetKeyboardState(lpKeyState);
		ToUnicode(wParam, HIWORD(lParam)&0xFF, lpKeyState, buffer, 1, 0);
		m_eventkey(KeyIndex,buffer[0],false);
		break;
	case WM_LBUTTONDOWN:
		m_eventmouse(MOUSE_LEFT,true);
		break;
	case WM_LBUTTONUP:
		m_eventmouse(MOUSE_LEFT,false);
		break;
	case WM_RBUTTONDOWN:
		m_eventmouse(MOUSE_RIGHT,true);
		break;
	case WM_RBUTTONUP:
		m_eventmouse(MOUSE_RIGHT,false);
		break;
	case WM_MBUTTONDOWN:
		m_eventmouse(MOUSE_MIDDLE,true);
		break;
	case WM_MBUTTONUP:
		m_eventmouse(MOUSE_MIDDLE,false);
		break;
	case WM_MOUSEWHEEL:
		m_mousewheel( (short)GET_WHEEL_DELTA_WPARAM(wParam) / WHEEL_DELTA );
		break;
	}
}

void InputManager::m_eventcursor()
{
	POINT Position;
	GetCursorPos(&Position);	// получаем текущую позицию курсора

	Position.x -= m_windowrect.left;
	Position.y -= m_windowrect.top;

	if (m_curx==Position.x && m_cury==Position.y)
		return;

	m_curx = Position.x;
	m_cury = Position.y;

	for (auto listener : m_Listener) {
		if (listener->MouseMove(MouseEvent(m_curx, m_cury))) return;
	}
}

void InputManager::m_eventmouse(const eMouseKeyCodes Code, bool press)
{
	for(auto listener : m_Listener)
	{
		if (press==true)
		{
			if (listener->MousePressed(MouseEventClick(Code, m_curx,m_cury))==true)
				return;
		}
		else
		{
			if (listener->MouseReleased(MouseEventClick(Code, m_curx,m_cury))==true)
				return;
		}
	}
}

void InputManager::m_mousewheel(short Value)
{
	if (m_MouseWheel==Value)
		return;

	m_MouseWheel = Value;

	for(auto listener : m_Listener)
	{
		if (listener->MouseWheel(MouseEventWheel(m_MouseWheel, m_curx,m_cury))==true)
			return;
	}
}

void InputManager::m_eventkey(const eKeyCodes KeyCode, const wchar_t ch, bool press)
{
	for(auto listener : m_Listener)
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