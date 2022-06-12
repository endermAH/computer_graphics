#include "Game.h"
#include "TriangleComponent.h"

Game::Game() {
	display_ = new Display(L"Default game window", 800, 800);
	PrepareDevice();
	CreateBackBuffer();
	Init();
}

void Game::Init() {
	DirectX::XMFLOAT4 points[6] = {
		DirectX::XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f),	DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f),
		DirectX::XMFLOAT4(-0.5f, -0.5f, 0.5f, 1.0f),	DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f),
		DirectX::XMFLOAT4(0.5f, -0.5f, 0.5f, 1.0f),	DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f)
	};
	DirectX::XMFLOAT4 points2[6] = {
		DirectX::XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f),	DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f),
		DirectX::XMFLOAT4(1.0f, 1.0f, 0.5f, 1.0f),	DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f),
		DirectX::XMFLOAT4(0.5f, 1.0f, 0.5f, 1.0f),	DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f)
	};
	components_.push_back(new TriangleComponent(this, points));
	components_.push_back(new TriangleComponent(this, points2));
}

void Game::Run() {
	MSG msg = {};
	exit_requested_ = false;

	CD3D11_RASTERIZER_DESC rastDesc = {};
	rastDesc.CullMode = D3D11_CULL_NONE;
	rastDesc.FillMode = D3D11_FILL_SOLID;

	ID3D11RasterizerState* rastState;
	auto res = device_->CreateRasterizerState(&rastDesc, &rastState);

	context_->RSSetState(rastState);
	context_->OMSetRenderTargets(1, &rtv_, nullptr);

	D3D11_VIEWPORT viewport = {};
	viewport.Width = static_cast<float>(display_->window_width_);
	viewport.Height = static_cast<float>(display_->window_height_);
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.MinDepth = 0;
	viewport.MaxDepth = 1.0f;

	context_->RSSetViewports(1, &viewport);

	float color[] = { 0.1f, 0.1f, 0.1f, 1.0f };
	
	while (!exit_requested_) {
		HandleMessages(msg);
		Update();

		//context_->ClearState();

		//context_->RSSetState(rastState);

		context_->ClearRenderTargetView(rtv_, color);
		
		for (auto &component : components_) {
			component->Update();
		}
		

		UINT strides[] = { 32 };
		UINT offsets[] = { 0 };

		/*ID3D11Buffer* buffers[2];
		buffers[0] = static_cast<TriangleComponent*>(components_[0])->vertex_buffer_;
		buffers[1] = static_cast<TriangleComponent*>(components_[1])->vertex_buffer_;*/

		ID3D11Buffer* ib;
		int indeces[] = { 0,1,2, 3,2,1 };
		D3D11_BUFFER_DESC indexBufDesc = {};
		indexBufDesc.Usage = D3D11_USAGE_DEFAULT;
		indexBufDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		indexBufDesc.CPUAccessFlags = 0;
		indexBufDesc.MiscFlags = 0;
		indexBufDesc.StructureByteStride = 0;
		indexBufDesc.ByteWidth = sizeof(int) * std::size(indeces);

		D3D11_SUBRESOURCE_DATA indexData = {};
		indexData.pSysMem = indeces;
		indexData.SysMemPitch = 0;
		indexData.SysMemSlicePitch = 0;

		device_->CreateBuffer(&indexBufDesc, &indexData, &ib);
		
		context_->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		context_->IASetVertexBuffers(0, 1, vb, strides, offsets);
		context_->IASetIndexBuffer(ib, DXGI_FORMAT_R32_UINT, 0);
		vb_count_ = 0;

		
		
		context_->DrawIndexed(6, 0, 0);
		//context_->Draw(6, 0);
		
		//context_->OMSetRenderTargets(0, nullptr, nullptr);

		swap_сhain_->Present(1, /*DXGI_PRESENT_DO_NOT_WAIT*/ 0);
	}
}

void Game::HandleMessages(MSG &message) {
	while (PeekMessage(&message, nullptr, 0, 0, PM_REMOVE)) {
		TranslateMessage(&message);
		DispatchMessage(&message);
	}
	if (message.message == WM_QUIT) {
		exit_requested_ = true;
	}
}

void Game::Update() {

}

void Game::PrepareDevice() {
	D3D_FEATURE_LEVEL featureLevel[] = { D3D_FEATURE_LEVEL_11_1 };

	DXGI_SWAP_CHAIN_DESC swapDesc = {};
	swapDesc.BufferCount = 2;
	swapDesc.BufferDesc.Width = display_->window_width_;
	swapDesc.BufferDesc.Height = display_->window_height_;
	swapDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapDesc.BufferDesc.RefreshRate.Numerator = 60;
	swapDesc.BufferDesc.RefreshRate.Denominator = 1;
	swapDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	swapDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapDesc.OutputWindow = display_->hWnd_;
	swapDesc.Windowed = true;
	swapDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;//DXGI_SWAP_EFFECT_FLIP_DISCARD;
	swapDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	swapDesc.SampleDesc.Count = 1;
	swapDesc.SampleDesc.Quality = 0;

	auto res = D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		D3D11_CREATE_DEVICE_DEBUG,
		featureLevel,
		1,
		D3D11_SDK_VERSION,
		&swapDesc,
		&swap_сhain_,
		&device_,
		nullptr,
		&context_);

	if (FAILED(res))
	{
		MessageBox(display_->hWnd_, L"Error", L"Can not create device or swapchain", MB_OK);
	}

}

void Game::CreateBackBuffer() {
	swap_сhain_->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&back_buffer_);
	device_->CreateRenderTargetView(back_buffer_, nullptr, &rtv_);
}