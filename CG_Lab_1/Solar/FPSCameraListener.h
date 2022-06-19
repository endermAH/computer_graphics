#pragma once

#include "../Framework/stdafx.h"
#include "../Framework/InputSystem/InputListener.h"

class FPSCameraListener: public InputListener
{
public:
	float camera_speed_ = 0.1;
	float mouse_x_;
	float mouse_y_;
	float mouse_delta_x_;
	float mouse_delta_y_;

	bool skip_next_ = false;
public:
	FPSCameraListener(float start_x, float start_y){
		mouse_x_ = start_x;
		mouse_y_ = start_y;
	};
	bool MouseMove(const MouseEvent& arg) override;
};

inline bool FPSCameraListener::MouseMove(const MouseEvent &arg) {
	if (skip_next_) {
		skip_next_ = false;
		Log::LogDebug(std::to_string(arg.y));
		return true;
	}
	mouse_delta_x_ = (mouse_x_ - static_cast<float>(arg.x)) * camera_speed_;
	mouse_delta_y_ = (mouse_y_ - static_cast<float>(arg.y)) * camera_speed_;
	Log::LogDebug(std::to_string(mouse_delta_y_));
	mouse_x_ = static_cast<float>(arg.x);
	mouse_y_ = static_cast<float>(arg.y);

	
	//SetCursorPos(520,250);
	//skip_next_ = true;
	return true;
}


