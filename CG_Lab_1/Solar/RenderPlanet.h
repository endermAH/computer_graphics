#pragma once
#include "../Framework/stdafx.h"
#include "../Framework/RenderSystem/RenderObject.h"
#include "../Framework/RenderSystem/RenderSystem.h"

struct PlanetColors
{
	DirectX::XMFLOAT4 center;
	DirectX::XMFLOAT4 border;
	DirectX::XMFLOAT4 main;
};

class RenderPlanet : public RenderObject
{
public:
	float radius_;
	PlanetColors colors_;
public:
	RenderPlanet(float radius_, PlanetColors colors);
	bool Init();
	bool Draw(RenderSystem* render_system);
	bool Release();
};
