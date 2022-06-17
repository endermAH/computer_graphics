#pragma once
#include "../stdafx.h"
#include "../RenderSystem/RenderObject.h"

//class RenderObject;

enum class GameObjectStates
{
	Creating, Active, PendingToKill
};

class GameObject
{
public:
	RenderObject *render_object_;
	int uid_;
	GameObjectStates state_ = GameObjectStates::Creating;
public:
	virtual void Update() = 0;

	void SetUid(int uid) {
		uid_ = uid;
	};

	virtual ~GameObject() {
		Log::LogDebug("Game object " + std::to_string(uid_) + " destroyed.");
		render_object_->Release();
	}
};