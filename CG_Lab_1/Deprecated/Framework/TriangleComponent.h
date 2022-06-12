#pragma once

#include "GameComponent.h"
#include "Game.h"

#include <d3d.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <directxmath.h>
#include <chrono>
#include <vector>
#include <iostream>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "dxguid.lib")

class TriangleComponent : public GameComponent
{
public:
	ID3D11VertexShader* vertex_shader_;
	ID3D11PixelShader* pixel_shader_;
	ID3D11InputLayout* layout_;
	ID3DBlob* vertexBC_ = nullptr;
	DirectX::XMFLOAT4 *points_;
	ID3D11Buffer* index_buffer_;
	ID3D11Buffer* vertex_buffer_;
public:
	TriangleComponent(Game* game, DirectX::XMFLOAT4 points[6]);
	void Init();
private:
	void Update();
	void CompileShaders();
	void CheckCompileResult(HRESULT res, ID3DBlob* error_code);
	void CreateLayout();
	void Draw();
};

