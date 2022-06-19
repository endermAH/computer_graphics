#include "RenderBall.h"

RenderBall::RenderBall(float width, float height) {
	height_ = height;
	width_ = width;
	Init();	
}

bool RenderBall::Init() {
	world_ = DirectX::XMMatrixIdentity();
	Log::LogDebug("Render Paddle Initialized");
	return true;
}

bool RenderBall::Draw(RenderSystem* render_system) {
	HRESULT hr = S_OK;

	float x_offset =  width_ / 2;
	float y_offset =  height_ / 2;
	
	SimpleVertex vertices[] =
	{
		{ DirectX::XMFLOAT3( 0, y_offset, 0.0f ), DirectX::XMFLOAT4( 1.0f, 1.0f, 1.0f, 1.0f ) },
		{ DirectX::XMFLOAT3( 0.75f * x_offset, 0.75f * y_offset, 0.0f ), DirectX::XMFLOAT4( 1.0f, 1.0f, 1.0f, 1.0f ) },
		{ DirectX::XMFLOAT3( x_offset, 0, 0.0f ), DirectX::XMFLOAT4( 1.0f, 1.0f, 1.0f, 1.0f ) },
		{ DirectX::XMFLOAT3(  0.75f * x_offset, -0.75f * y_offset, 0.0f ), DirectX::XMFLOAT4( 1.0f, 1.0f, 1.0f, 1.0f ) },
		{ DirectX::XMFLOAT3( 0, -y_offset, 0.0f ), DirectX::XMFLOAT4( 1.0f, 1.0f, 1.0f, 1.0f ) },
		{ DirectX::XMFLOAT3( -0.75f * x_offset, -0.75f * y_offset, 0.0f ), DirectX::XMFLOAT4( 1.0f, 1.0f, 1.0f, 1.0f ) },
		{ DirectX::XMFLOAT3( -x_offset, 0, 0.0f ), DirectX::XMFLOAT4( 1.0f, 1.0f, 1.0f, 1.0f ) },
		{ DirectX::XMFLOAT3( -0.75f * x_offset,  0.75f * y_offset, 0.0f ), DirectX::XMFLOAT4( 1.0f, 1.0f, 1.0f, 1.0f ) },
		{ DirectX::XMFLOAT3( 0, 0, 0.0f ), DirectX::XMFLOAT4( 1.0f, 1.0f, 1.0f, 1.0f ) },
	};

	D3D11_BUFFER_DESC bd;
	ZeroMemory( &bd, sizeof(bd) );
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof( SimpleVertex ) * 9;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	
	D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory( &InitData, sizeof(InitData) );
	InitData.pSysMem = vertices;
	hr = render_system->device_->CreateBuffer(&bd, &InitData, &vertex_buffer_);
	if( FAILED( hr ) ) {
		Log::LogError("Failed to create vertex buffer");
		return false;
	}
		

	UINT stride = sizeof( SimpleVertex );
	UINT offset = 0;
	render_system->immediate_context_->IASetVertexBuffers( 0, 1, &vertex_buffer_, &stride, &offset);

	WORD indices[] =
	{
		0,1,8,
		1,2,8,
		2,3,8,
		3,4,8,
		4,5,8,
		5,6,8,
		6,7,8,
		7,0,8,
	};

	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof( WORD ) * 24;
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;
	
	InitData.pSysMem = indices;
	hr = render_system->device_->CreateBuffer( &bd, &InitData, &index_buffer_ );
	if( FAILED( hr ) ) {
		Log::LogError("Failed to create index buffer");
		return false;
	}

	render_system->immediate_context_->IASetIndexBuffer( index_buffer_, DXGI_FORMAT_R16_UINT, 0 );

	render_system->immediate_context_->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(ConstantBuffer);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;

	hr = render_system->device_->CreateBuffer( &bd, NULL, &constant_buffer_ );
	if(FAILED(hr)) return false;
	
	ConstantBuffer cb1;
	cb1.mWorld = DirectX::XMMatrixTranspose( world_ );
	cb1.mView = DirectX::XMMatrixTranspose( render_system->view_ );
	cb1.mProjection = DirectX::XMMatrixTranspose( render_system->projection_ );
	render_system->immediate_context_->UpdateSubresource( constant_buffer_, 0, NULL, &cb1, 0, 0 );

	render_system->immediate_context_->VSSetShader( render_system->vertex_shader_, NULL, 0 );
	render_system->immediate_context_->VSSetConstantBuffers( 0, 1, &constant_buffer_ );
	render_system->immediate_context_->PSSetShader( render_system->pixel_shader_, NULL, 0 );
	render_system->immediate_context_->DrawIndexed( 24, 0, 0 );

	vertex_buffer_->Release();
	index_buffer_->Release();
	constant_buffer_->Release();
	//Log::LogDebug("Drawing 0");
}

bool RenderBall::Release() {
	/*vertex_buffer_->Release();
	index_buffer_->Release();
	constant_buffer_->Release();*/
	return true;
}