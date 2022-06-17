#pragma once
#include "Ball.h"
#include "Paddle.h"
#include "../Framework/GameSystem/Game.h"

class Pong : public Game
{
public:
	int top_score = 0;
	int bot_score = 0;

	Paddle *paddle_top_;
	Paddle *paddle_bot_;
	Ball *ball_;
	
public:
	Pong(float window_width, float window_height)
		: Game(window_width, window_height) {}
	
	void GameStart() {
		paddle_top_ = new Paddle(200.0, eKeyCodes::KEY_RIGHT, eKeyCodes::KEY_LEFT);
		paddle_bot_ = new Paddle(-200.0, eKeyCodes::KEY_D, eKeyCodes::KEY_A);
		ball_ = new Ball(paddle_top_, paddle_bot_, 0, 0);

		AddObject(paddle_top_);
		AddObject(paddle_bot_);
		AddObject(ball_);
		
		Run();
		
		Close();
	}
	
	bool GameFrame() {
		if (ball_->is_outside_ && ball_->is_active_) {
			Ball *new_ball;
			if (ball_->y_ > 0) {
				bot_score += 1;
				Log::LogInfo("Bot player wins a round! Score top:bot - " + std::to_string(top_score) + ":" + std::to_string(bot_score));
				new_ball = new Ball(paddle_top_, paddle_bot_, 0, -150);
			} else {
				top_score += 1;
				Log::LogInfo("Top player wins a round! Score top:bot - " + std::to_string(top_score) + ":" + std::to_string(bot_score));
				new_ball = new Ball(paddle_top_, paddle_bot_, 0, 150);
			}
			Log::LogInfo("Ball -  " + std::to_string(ball_->x_) + ":" + std::to_string(ball_->y_));
			DestroyObject(ball_);
			ball_ = new_ball;
			AddObject(new_ball);
			
			if (top_score == 3 || bot_score == 3) {
				Log::LogInfo("Game finished! Score top:bot - " + std::to_string(top_score) + ":" + std::to_string(bot_score));
				return false;
			}
		}
		return true;
	};
};
