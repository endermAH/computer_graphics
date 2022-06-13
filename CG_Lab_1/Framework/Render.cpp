#include "stdafx.h"
#include "Render.h"

Render::Render()
{
	driver_type_ = D3D_DRIVER_TYPE_NULL;
	feature_level_ = D3D_FEATURE_LEVEL_11_0;
	device_ = nullptr;
	immediate_context_ = nullptr;
	swap_chain_ = nullptr;
	render_target_view_ = nullptr;
	m_pDepthStencil = nullptr;
	m_pDepthStencilView = nullptr;
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

	D3D11_TEXTURE2D_DESC descDepth;
	ZeroMemory( &descDepth, sizeof(descDepth) );
	descDepth.Width = width;
	descDepth.Height = height;
	descDepth.MipLevels = 1;
	descDepth.ArraySize = 1;
	descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	descDepth.SampleDesc.Count = 1;
	descDepth.SampleDesc.Quality = 0;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	descDepth.CPUAccessFlags = 0;
	descDepth.MiscFlags = 0;
	hr = device_->CreateTexture2D( &descDepth, NULL, &m_pDepthStencil );
	if( FAILED( hr ) )
		return false;

	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
	ZeroMemory( &descDSV, sizeof(descDSV) );
	descDSV.Format = descDepth.Format;
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	descDSV.Texture2D.MipSlice = 0;
	hr = device_->CreateDepthStencilView(m_pDepthStencil, &descDSV, &m_pDepthStencilView);
	if( FAILED( hr ) )
		return false;

	immediate_context_->OMSetRenderTargets( 1, &render_target_view_, m_pDepthStencilView  );

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
	immediate_context_->ClearDepthStencilView( m_pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0 );
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

	m_pDepthStencil->Release();
	m_pDepthStencilView->Release();
	render_target_view_->Release();
	swap_chain_->Release();
	immediate_context_->Release();
	device_->Release();
}

HRESULT Render::CompileShaderFromFile(std::string FileName, LPCSTR EntryPoint, LPCSTR ShaderModel, ID3DBlob** ppBlobOut)
{
	HRESULT hr = S_OK;
	ID3DBlob* errorCode = nullptr;
	std::wstring wide_string = std::wstring(FileName.begin(), FileName.end());
		
	hr = D3DCompileFromFile(wide_string.c_str(),
		nullptr,
		nullptr,
		EntryPoint,
		ShaderModel,
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
		0,
		ppBlobOut, //vertexBc???
		&errorCode);

	CheckCompileResult(hr, errorCode);

	return hr;
}

void Render::CheckCompileResult(HRESULT res, ID3DBlob* error_code) {
	if (FAILED(res)) {
		if (error_code) {
			char* compileErrors = (char*)(error_code->GetBufferPointer());
			Log::LogError("Can not compile \"../Shaders/default.hlsl\".");
			Log::LogError(compileErrors);
		}
		else
		{
			Log::LogError("Missing shader file");
		}
	}
}