#include "RenderSystem.h"

RenderSystem::RenderSystem(ProjectionType projection_type) {
	vertex_layout_ = nullptr;
	vertex_shader_ = nullptr;
	pixel_shader_ = nullptr;
	
	projection_type_ = projection_type;
}

bool RenderSystem::Init(HWND hwnd) {
	HRESULT hr = S_OK;
	ID3DBlob* vs_blob = NULL;

	// TODO: Create vertex shader abstraction
	hr = CompileShaderFromFile("solar.hlsl", "VS", "vs_5_0", &vs_blob );
	if(FAILED(hr)) return false;

	hr = device_->CreateVertexShader( vs_blob->GetBufferPointer(), vs_blob->GetBufferSize(), NULL, &vertex_shader_ );
	if(FAILED(hr)) {	
		Log::LogError("Can not create vertex shader");
		vs_blob->Release();
		return false;
	}

	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{
			"POSITION",
			0,
			DXGI_FORMAT_R32G32B32_FLOAT,
			0,
			0,
			D3D11_INPUT_PER_VERTEX_DATA,
			0
		},
		{
			"COLOR",
			0,
			DXGI_FORMAT_R32G32B32A32_FLOAT,
			0,
			12,
			D3D11_INPUT_PER_VERTEX_DATA,
			0
		},
	};

	UINT elements_count = ARRAYSIZE(layout);

	hr = device_->CreateInputLayout(
		layout,
		elements_count,
		vs_blob->GetBufferPointer(),
		vs_blob->GetBufferSize(),
		&vertex_layout_);
	vs_blob->Release();
	
	if(FAILED(hr)) {
		Log::LogError("Can not create a layout");
		return false;
	}

	immediate_context_->IASetInputLayout( vertex_layout_ );

	ID3DBlob* ps_blob = NULL;
	hr = CompileShaderFromFile( "solar.hlsl", "PS", "ps_5_0", &ps_blob );
	if(FAILED(hr)) return false;

	// TODO: Create pixel shader abstraction
	hr = device_->CreatePixelShader( ps_blob->GetBufferPointer(), ps_blob->GetBufferSize(), NULL, &pixel_shader_ );
	ps_blob->Release();
	if( FAILED( hr ) )
	{	
		Log::LogError("Can not create pixel shader");
		ps_blob->Release();
		return false;
	}

	eye_ = DirectX::XMVectorSet( 0.0f, 5.0f, -5.0f, 0.0f );
	at_ = DirectX::XMVectorSet( 0.0f, 0.0f, 0.0f, 0.0f );
	up_ = DirectX::XMVectorSet( 0.0f, 1.0f, 0.0f, 0.0f );
	view_ = DirectX::XMMatrixLookAtLH( eye_, at_, up_ );

	// TODO: Move to Game Object
	float width = Game::GetWindowWidth();
	float height = Game::GetWindowHeight();

	if (projection_type_ == ProjectionType::Orthographic) {
		projection_ = DirectX::XMMatrixOrthographicLH(width, height, 0.01f, 100.0f);
	} else if (projection_type_ == ProjectionType::Perspective) {
		projection_  = DirectX::XMMatrixPerspectiveFovLH(DirectX::XM_PIDIV2, width/height, 0.01f, 100.0f );
	}
	
	return true;
}

bool RenderSystem::Draw() {
	//view_ = DirectX::XMMatrixLookAtLH( eye_, at_, up_ );
	auto* game_objects = Game::GetAllObjects();
	
	for (int i = 0; i < game_objects->size(); i++) {
		// Log::LogDebug(std::to_string( i));
		game_objects->at(i)->render_object_->Draw(this);
	}
	
	return true;
}

void RenderSystem::Update() {
	return;
}

void RenderSystem::Close() {
	return;
}



