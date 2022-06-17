#pragma once

#include "../stdafx.h"
#include "GameObject.h"
#include "../Framework.h"
#include "../../EDUC/RenderRotatingCubes.h"
#include "../RenderSystem/RenderSystem.h"

class Game : public Framework
{
private:
	static Game* main_instance_;
	
	KeyboardController* keyboard_controller_ ;
	std::vector<GameObject*>* game_objects_ ;

	float window_width_;
	float window_height_;

	int object_count_;
	
	//RenderSystem* render_;
public:
	Game(float window_width, float window_height);
	static std::vector<GameObject*>* GetAllObjects() {
		return main_instance_->game_objects_;
	}
	static KeyboardController* Keyboard() {
		return main_instance_->keyboard_controller_;
	}
	void AddObject(GameObject* game_object) {
		game_object->SetUid(object_count_);
		game_object->state_ = GameObjectStates::Active;
		object_count_++;
		game_objects_->push_back(game_object);
	}

	float static GetWindowWidth() {
		return main_instance_->window_width_;
	}

	float static GetWindowHeight() {
		return main_instance_->window_height_;
	}

	void static DestroyObject(GameObject* object) {
		std::vector<GameObject*>* objects = main_instance_->game_objects_;
		//objects->erase(objects->begin() + 2);
		
		for (int i = 0; i < objects->size(); i++) {
			if (objects->at(i)->uid_ == object->uid_) {
				Log::LogDebug(std::to_string(object->uid_));
				objects->at(i)->state_ = GameObjectStates::PendingToKill;
				objects->erase(objects->begin() + 2);
				delete object;
				//delete objects->at(i);
			}	
		}
	}

	virtual bool GameFrame() {
		return true;
	}

protected:
	bool frame_() override;
};

#pragma once
