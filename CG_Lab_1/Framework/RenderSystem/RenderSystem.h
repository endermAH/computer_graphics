#pragma once
#include "../stdafx.h"
#include "Render.h"
#include "../GameSystem/Game.h"
#include "../GameSystem/GameObject.h"
//class GameObject;

class RenderSystem : public Render
{
public:
	ProjectionType projection_type_;
	
	ID3D11VertexShader *vertex_shader_;
	ID3D11PixelShader *pixel_shader_;
	ID3D11InputLayout *vertex_layout_;	

	DirectX::XMVECTOR eye_;
	DirectX::XMVECTOR at_;
	DirectX::XMVECTOR up_;
	
	DirectX::XMMATRIX view_;
	DirectX::XMMATRIX projection_;
public:
	RenderSystem(ProjectionType projection_type);
	bool Init(HWND hwnd);
	bool Draw();
	void Update();
	void Close();

	void* operator new(size_t i)
	{
	return _aligned_malloc(i,16);
	}

	void operator delete(void* p)
	{
		_aligned_free(p);
	}
};
