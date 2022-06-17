#pragma once
#include "../Framework/stdafx.h"
#include "../Framework/RenderSystem/RenderObject.h"
#include "../Framework/RenderSystem/RenderSystem.h"

class RenderBall : public RenderObject
{
public:
	float height_;
	float width_;
public:
	RenderBall(float width, float height);
	bool Init();
	bool Draw(RenderSystem* render_system);
	bool Release();
};
