#include "Render.h"

Render::Render()
{
	driver_type_ = D3D_DRIVER_TYPE_NULL;
	feature_level_ = D3D_FEATURE_LEVEL_11_0;
	device_ = nullptr;
	immediate_context_ = nullptr;
	swap_chain_ = nullptr;
	render_target_view_ = nullptr;
}

Render::~Render()
{
}

bool Render::CreateDevice(HWND hwnd) {
	HRESULT hr = S_OK;

	RECT rc;
	GetClientRect( hwnd, &rc );
	UINT width = rc.right - rc.left;
	UINT height = rc.bottom - rc.top;

	UINT create_device_flags = 0;
#ifdef _DEBUG
	create_device_flags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	D3D_DRIVER_TYPE driver_types[] =
	{
		D3D_DRIVER_TYPE_HARDWARE,
		D3D_DRIVER_TYPE_WARP,
		D3D_DRIVER_TYPE_REFERENCE,
	};
	UINT driver_types_count = ARRAYSIZE(driver_types);

	D3D_FEATURE_LEVEL feature_levels[] =
	{
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
	};
	UINT feature_levels_count = ARRAYSIZE(feature_levels);

	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory( &sd, sizeof( sd ) );	
	sd.BufferCount = 1;			
	sd.BufferDesc.Width = width;		
	sd.BufferDesc.Height = height;		
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; 
	sd.BufferDesc.RefreshRate.Numerator = 60; 
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = hwnd;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.Windowed = TRUE;

	for (UINT driver_type_index = 0; driver_type_index < driver_types_count; driver_type_index++)
	{
		driver_type_ = driver_types[driver_type_index];
		hr = D3D11CreateDeviceAndSwapChain( NULL, driver_type_, 
			NULL, create_device_flags, 
			feature_levels, feature_levels_count, 
			D3D11_SDK_VERSION, &sd, 
			&swap_chain_, &device_, 
			&feature_level_, &immediate_context_ );
		if( SUCCEEDED( hr ) )
			break;
	}
	if (FAILED(hr)) return false;

	ID3D11Texture2D* back_buffer = NULL;
	hr = swap_chain_->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&back_buffer);
	if (FAILED(hr)) return false;

	hr = device_->CreateRenderTargetView(back_buffer, NULL, &render_target_view_);
	back_buffer->Release();
	if (FAILED(hr)) return false;

	immediate_context_->OMSetRenderTargets( 1, &render_target_view_, NULL );

	D3D11_VIEWPORT viewport;
	viewport.Width = (FLOAT)width;
	viewport.Height = (FLOAT)height;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	immediate_context_->RSSetViewports( 1, &viewport );

	return Init(hwnd);
}

void Render::BeginFrame()
{
	float clear_color[4] = { 0.0f, 0.125f, 0.3f, 1.0f };
	immediate_context_->ClearRenderTargetView(render_target_view_, clear_color);
}

void Render::EndFrame()
{
	swap_chain_->Present( 0, 0 );
}

void Render::Shutdown()
{
	Close();

	if( immediate_context_) 
		immediate_context_->ClearState();

	render_target_view_->Release();
	swap_chain_->Release();
	immediate_context_->Release();
	device_->Release();
}