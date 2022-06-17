#pragma once
#include "../stdafx.h"
#include "Render.h"
#include "../GameSystem/Game.h"
#include "../GameSystem/GameObject.h"
//class GameObject;
class RenderSystem : public Render
{
public:
	ID3D11VertexShader *vertex_shader_;
	ID3D11PixelShader *pixel_shader_;
	ID3D11InputLayout *vertex_layout_;	

	DirectX::XMMATRIX view_;
	DirectX::XMMATRIX projection_;
public:
	RenderSystem();
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
