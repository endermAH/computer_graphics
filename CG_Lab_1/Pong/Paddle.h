#pragma once

#include "RenderPaddle.h"
#include "../Framework/stdafx.h"
#include "../Framework/GameSystem/GameObject.h"
#include "../Framework/InputSystem/InputCodes.h"


class Paddle : public GameObject
{
public:
	const float width_ = 100;
	const float height_ = 30;

	const float speed_ = 0.5;

	eKeyCodes key_right_;
	eKeyCodes key_left_;
	
	float y_offset_;
	float x_ = 0;
	
	Paddle(float y_offset, eKeyCodes key_right, eKeyCodes key_left) {
		render_object_ = new RenderPaddle(width_, height_);
		y_offset_ = y_offset;
		key_left_ = key_left;
		key_right_ = key_right;
	}
	
	void Update() {
		static float t = 0.0f;
		static DWORD dwTimeStart = 0;
		DWORD dwTimeCur = GetTickCount();
		if( dwTimeStart == 0 )
			dwTimeStart = dwTimeCur;
		t = ( dwTimeCur - dwTimeStart ) / 1000.0f;

		if (Game::Keyboard()->IsKeyDown(key_right_)) {
			SetX(x_ + speed_);
		}

		if (Game::Keyboard()->IsKeyDown(key_left_)) {
			SetX(x_ - speed_);
		}
		
		render_object_->world_ = DirectX::XMMatrixTranslation( x_ , y_offset_, 0.0f );
		
	}

private:
	
	void SetX(float x) {
		float new_x = x;
		float right_max = Game::GetWindowWidth() / 2 - width_ / 2;
		float left_max = width_ / 2 - Game::GetWindowWidth() / 2;
		if (new_x < left_max) new_x = left_max;
		if (new_x > right_max) new_x =  right_max;
		x_ = new_x;
	}
};
