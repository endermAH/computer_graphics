#pragma once

#include <unordered_set>

#include "InputListener.h"
#include "InputCodes.h"
#include "../Log.h"

class KeyboardController : public InputListener
{
private:
	std::unordered_set<unsigned int> keys_down;
public:
	bool KeyPressed(const KeyEvent &arg)
	{ 
		Log::LogDebug("Key pressed: " + std::to_string(char(arg.code)));
		keys_down.insert(static_cast<unsigned int>(arg.code));
		return false; 
	}
	bool KeyReleased(const KeyEvent& arg) {
		Log::LogDebug("Key realised: " + std::to_string(char(arg.code)));
		keys_down.erase(static_cast<unsigned int>(arg.code));
		return false; 
	}

	bool IsKeyDown(eKeyCodes key) {
		return (bool)keys_down.count(static_cast<unsigned int>(key));
	}
};

class MouseController : public InputListener
{
public:
	float mouse_x_;
	float mouse_y_;
public:
	bool MouseMove(const MouseEvent& arg) {
		mouse_x_ = arg.x;
		mouse_y_ = arg.y;
		return true;
	};
};