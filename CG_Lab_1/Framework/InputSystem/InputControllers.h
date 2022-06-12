#pragma once

#include <unordered_set>

#include "InputListener.h"
#include "InputCodes.h"
#include "../Log.h"

class KeyboardController : public InputListener
{
public:
	bool KeyPressed(const KeyEvent &arg)
	{ 
		Log::LogInfo("Key pressed: " + std::to_string(char(arg.code)));
		keys_down.insert(arg.code);
		return false; 
	}
	bool KeyReleased(const KeyEvent& arg) {
		Log::LogInfo("Key realised: " + std::to_string(char(arg.code)));
		keys_down.erase(arg.code);
		return false; 
	}

	bool IsKeyDown(eKeyCodes key) {
		return keys_down.count(key);
	}
private:
	std::unordered_set<eKeyCodes> keys_down;
};
