#pragma once

#include "Paddle.h"
#include "RenderBall.h"
#include "RenderPaddle.h"
#include "../Framework/stdafx.h"
#include "../Framework/GameSystem/GameObject.h"

class Ball : public GameObject
{
public:
	const float width_ = 30.0f;
	const float height_ = 30.0f;
	const float start_speed_ = 0.2f;

	float static_speed_x_ = 0.2f;
	float static_speed_y_ = 0.2f;

	Paddle* paddle_top_;
	Paddle* paddle_bot_;
	
	float cur_speed_x_ = static_speed_x_;
	float cur_speed_y_ = static_speed_y_;
	
	float x_ = 0;
	float y_ = 0;

	bool is_outside_ = false;
	bool is_active_ = true;
	
	Ball(Paddle* paddle_top, Paddle* paddle_bot, float x, float y) {
		render_object_ = new RenderBall(width_, height_);
		paddle_top_ = paddle_top;
		paddle_bot_ = paddle_bot;
		x_ = x;
		y_ = y;
		static_speed_x_ = start_speed_ * std::rand()/((RAND_MAX + 1u)/100) / 100;
		static_speed_y_ = start_speed_ - static_speed_x_;
		cur_speed_x_ = static_speed_x_;
		cur_speed_y_ = static_speed_y_;
	}
	
	void Update() {
		if (is_outside_) {
			is_active_ = false;
			return;
		};
		
		static float t = 0.0f;
		static DWORD dwTimeStart = 0;
		DWORD dwTimeCur = GetTickCount();
		if( dwTimeStart == 0 )
			dwTimeStart = dwTimeCur;
		t = ( dwTimeCur - dwTimeStart ) / 1000.0f;

		SetPos(x_ + cur_speed_x_, y_ + cur_speed_y_);
		
		render_object_->world_ = DirectX::XMMatrixTranslation( x_ , y_, 0.0f );
	}



private:
	void SetPos(float x, float y) {
		float new_x = x;
		float right_max = Game::GetWindowWidth() / 2 - width_ / 2;
		float left_max = width_ / 2 - Game::GetWindowWidth() / 2;
		if (new_x < left_max) cur_speed_x_ = cur_speed_x_ * -1;
		if (new_x > right_max) cur_speed_x_ = cur_speed_x_ * -1;
		x_ = new_x;

		float new_y = y;
		float top_max = Game::GetWindowHeight() / 2 - height_ / 2;
		float bot_max = height_ / 2 - Game::GetWindowHeight() / 2;
		
		float paddle_top_boreder = paddle_top_->y_offset_ - paddle_top_->height_ / 2 - height_ / 2;
		float paddle_bot_boreder = paddle_bot_->y_offset_ + paddle_bot_->height_ / 2 + height_ / 2;

		if (new_y > paddle_top_boreder && IsOnPaddle(paddle_top_) && cur_speed_y_ > 0) {
			static_speed_x_ *= 1.1f;
			static_speed_y_ *= 1.1f;
			cur_speed_y_ = static_speed_y_ * -1;

		}
		if (new_y < paddle_bot_boreder && IsOnPaddle(paddle_bot_) && cur_speed_y_ < 0) {
			static_speed_x_ += 0.01f;
			static_speed_y_ += 0.01f;
			cur_speed_y_ = static_speed_y_;
		}
		
		if (new_y > top_max) is_outside_ = true; 
		if (new_y < bot_max) is_outside_ = true;
		y_ = new_y;
	}

	bool IsOnPaddle(Paddle* paddle) {
		float min = paddle->x_ - paddle->width_ / 2;
		float max = paddle->x_ + paddle->width_ / 2;
		bool result = true;
		if (x_ > max) result = false;
		if (x_ < min) result = false;
		return result;
	}
};
