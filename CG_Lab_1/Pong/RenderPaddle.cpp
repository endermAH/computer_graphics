#include "RenderPaddle.h"

RenderPaddle::RenderPaddle(float width, float height) {
	height_ = height;
	width_ = width;
	Init();	
}

bool RenderPaddle::Init() {
	world_ = DirectX::XMMatrixIdentity();
	Log::LogDebug("Render Paddle Initialized");
	return true;
}

bool RenderPaddle::Draw(RenderSystem* render_system) {
	HRESULT hr = S_OK;

	float x_offset =  width_ / 2;
	float y_offset =  height_ / 2;
	
	SimpleVertex vertices[] =
	{
		{ DirectX::XMFLOAT3( -x_offset, -y_offset, 0.0f ), DirectX::XMFLOAT4( 1.0f, 1.0f, 1.0f, 1.0f ) },
		{ DirectX::XMFLOAT3( -x_offset, y_offset, 0.0f ), DirectX::XMFLOAT4( 1.0f, 1.0f, 1.0f, 1.0f ) },
		{ DirectX::XMFLOAT3( x_offset, -y_offset, 0.0f ), DirectX::XMFLOAT4( 1.0f, 1.0f, 1.0f, 1.0f ) },
		{ DirectX::XMFLOAT3( x_offset, y_offset, 0.0f ), DirectX::XMFLOAT4( 1.0f, 1.0f, 1.0f, 1.0f ) },
	};

	D3D11_BUFFER_DESC bd;
	ZeroMemory( &bd, sizeof(bd) );
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof( SimpleVertex ) * 4;
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
		2,1,3,
		0,1,2,
	};

	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof( WORD ) * 6;
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
	render_system->immediate_context_->DrawIndexed( 6, 0, 0 );

	vertex_buffer_->Release();
	index_buffer_->Release();
	constant_buffer_->Release();
	//Log::LogDebug("Drawing 0");
}

bool RenderPaddle::Release() {
	vertex_buffer_->Release();
	index_buffer_->Release();
	constant_buffer_->Release();
	return true;
}