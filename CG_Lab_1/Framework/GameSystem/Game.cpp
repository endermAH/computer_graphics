#include "Game.h"

Game *Game::main_instance_ = nullptr;

Game::Game(float window_width, float window_height) {
	std::srand(std::time(nullptr));
	
	main_instance_ = this;
	game_objects_ = new std::vector<GameObject*>;
		
	window_width_ = window_width;
	window_height_ = window_height;
	RenderSystem *render = new RenderSystem();
	
	FrameworkDescriptor framework_desc;
	framework_desc.window_descriptor.width = window_width_;
	framework_desc.window_descriptor.height = window_height_;
	framework_desc.render = render;

	
	Init(framework_desc);

	Log::LogDebug("Game class Initialized");
	
	keyboard_controller_ = new KeyboardController();
	AddInputListener(keyboard_controller_);
}

bool Game::frame_() {
	window_->RunEvent();
	if (!window_->IsActive())
		return true;
	
	if (window_->IsExit())
		return false;
	
	if (window_->IsResize())
	{
	}

	if (!GameFrame()) return false;
	
	for (int i = 0; i < game_objects_->size(); i++) {
		if (game_objects_->at(i)->state_ == GameObjectStates::Active) game_objects_->at(i)->Update();
	}
	
	render_->BeginFrame();

	// Log::LogDebug("Drawing objects");
	if (!render_->Draw()) {
		Log::LogError("Failed to Draw");
		return false;
	}
	render_->EndFrame();
	
	return true;
}