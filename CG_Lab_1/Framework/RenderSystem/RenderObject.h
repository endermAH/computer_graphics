#pragma once
//#include "RenderSystem.h"
#include "../stdafx.h"

class RenderSystem;

struct SimpleVertex
{
	DirectX::XMFLOAT3 Pos;
	DirectX::XMFLOAT4 Color;
};

struct ConstantBuffer
{
	DirectX::XMMATRIX mWorld;
	DirectX::XMMATRIX mView;
	DirectX::XMMATRIX mProjection;
};

class RenderObject
{
public:
	ID3D11Buffer *vertex_buffer_;
	ID3D11Buffer *index_buffer_;
	ID3D11Buffer *constant_buffer_;
	DirectX::XMMATRIX world_;
public:
	virtual bool Init() = 0;
	virtual bool Draw(RenderSystem *render_system) = 0;
	virtual bool Release() = 0;
};
