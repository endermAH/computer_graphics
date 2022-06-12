#pragma once

#include "InputCodes.h"

	struct MouseEvent
	{
		int x;				
		int y;
		MouseEvent(int new_x, int new_y) :  x(new_x), y(new_y) {}
	};


	struct MouseEventClick : public MouseEvent
	{
		const eMouseKeyCodes button;
		MouseEventClick(eMouseKeyCodes pressed_button, int x, int y) : MouseEvent(x,y), button(pressed_button) {}
	};


	struct MouseEventWheel : public MouseEvent
	{
		int wheel;
		MouseEventWheel(int new_wheel, int x, int y) : MouseEvent(x,y), wheel(new_wheel) {}
	};


	struct KeyEvent
	{
		const wchar_t wc;
		const eKeyCodes code;
		KeyEvent(wchar_t c, eKeyCodes kc) : wc(c), code(kc) {}
	};

	class InputListener
	{
	public:
		virtual bool MousePressed(const MouseEventClick &arg) { return false; }
		virtual bool MouseReleased(const MouseEventClick &arg) { return false; }
		virtual bool MouseWheel(const MouseEventWheel &arg) { return false; }
		virtual bool MouseMove(const MouseEvent &arg) {return false; }
		virtual bool KeyPressed(const KeyEvent &arg) { return false; }
		virtual bool KeyReleased(const KeyEvent &arg) { return false; }
	};
