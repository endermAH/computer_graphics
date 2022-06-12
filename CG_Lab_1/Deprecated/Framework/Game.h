#pragma once

#include "Display.h"
#include "GameComponent.h"

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

class Game
{
public:
	LPCWSTR name_ = L"Test name";
	Display *display_;
	//Microsoft::WRL::ComPtr<ID3D11Device> device_;
	ID3D11Device* device_;
	ID3D11DeviceContext* context_;
	IDXGISwapChain* swap_сhain_;
	ID3D11Texture2D* back_buffer_;
	ID3D11RenderTargetView* rtv_;
	ID3D11Buffer* vb[2];
	int vb_count_ = 0;
	std::vector<GameComponent*> components_;
private:
	bool exit_requested_;
public:
	Game();
	void Run();
	void Test() {
		std::cout << "TEST STRING";
	};
protected:
	void Init();
	void HandleMessages(MSG& message);
	void Update();
	void PrepareDevice();
	void CreateBackBuffer();
};

