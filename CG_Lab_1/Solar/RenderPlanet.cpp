#include "RenderPlanet.h"

RenderPlanet::RenderPlanet(float radius,  PlanetColors colors) {
	radius_ = radius;
	colors_ = colors;
	Init();	
}

bool RenderPlanet::Init() {
	world_ = DirectX::XMMatrixIdentity();
	Log::LogDebug("Render Planet Initialized");
	return true;
}

bool RenderPlanet::Draw(RenderSystem* render_system) {
	HRESULT hr = S_OK;

	float x_offset =  radius_ / 2;
	float y_offset =  radius_ / 2;

	std::vector<SimpleVertex> vertices;

	float start_point = -radius_;
	int circles_count = 50;
	int points_on_circle = 25;
	float pi = 3.14;

	vertices.push_back({ DirectX::XMFLOAT3( 0, 0, -radius_ ), colors_.center });
	for ( int i = 0; i < circles_count; i++ ) {
		DirectX::XMFLOAT4 Color;
		if ( i < circles_count / 20 ) {
			Color = colors_.center;
		} else if ( i < circles_count / 10 ) {
			Color = colors_.border;
		} else {
			Color = colors_.main;
		}
		float circle_y = start_point + (radius_ * 2 / (circles_count + 1)) * (i + 1);
		float circle_radius;
		if (circle_y == 0)
			circle_radius = radius_;
		else
			circle_radius = sqrt(pow(radius_,2) - pow(circle_y, 2));
		for (int j = 0; j < points_on_circle; j++) {
			float x = circle_radius * cos(2 * pi * j / points_on_circle);
			float y = circle_radius * sin(2 * pi * j / points_on_circle);
			float z = circle_y;
			vertices.push_back({ DirectX::XMFLOAT3( x, y, z ), Color });
		}
	}
	vertices.push_back({ DirectX::XMFLOAT3( 0, 0, radius_ ), colors_.main });
	
	D3D11_BUFFER_DESC bd;
	ZeroMemory( &bd, sizeof(bd) );
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof( SimpleVertex ) * vertices.size();
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	
	D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory( &InitData, sizeof(InitData) );
	InitData.pSysMem = vertices.data();
	hr = render_system->device_->CreateBuffer(&bd, &InitData, &vertex_buffer_);
	if( FAILED( hr ) ) {
		Log::LogError("Failed to create vertex buffer");
		return false;
	}
		

	UINT stride = sizeof( SimpleVertex );
	UINT offset = 0;
	render_system->immediate_context_->IASetVertexBuffers( 0, 1, &vertex_buffer_, &stride, &offset);

	std::vector<WORD> indices;
	int current_point=1;
	for (int i = 0; i < circles_count; i++) {
		if(i == 0) {
			for (int v = 0; v < points_on_circle; v++) {
				WORD i1 = 0;
				WORD i2 = current_point + v;
				WORD i3;
				if (v == points_on_circle - 1) i3 = current_point;
				else i3 = current_point+v+1;
				indices.push_back(i3);
				indices.push_back(i2);
				indices.push_back(i1);
			}
		} else if(i == circles_count - 1) {
			for (int v = 0; v < points_on_circle; v++) {
				WORD i1 = current_point + v - points_on_circle;
				WORD i2 = current_point + v - points_on_circle + 1;
				WORD i3 = vertices.size()-1;
				if (v == points_on_circle - 1) i2 = current_point - points_on_circle;
				else i2 = current_point + v - points_on_circle + 1;
				indices.push_back(i1);
				indices.push_back(i2);
				indices.push_back(i3);
			}
		} else {
			for (int v = 0; v < points_on_circle; v++) {
				WORD i1 = current_point + v - points_on_circle;
				WORD i2 = current_point + v;
				WORD i3;
				if (v == points_on_circle - 1) i3 = current_point;
				else i3 = current_point + v + 1;
				indices.push_back(i3);
				indices.push_back(i2);
				indices.push_back(i1);

				i1 = current_point + v - points_on_circle;
				if (v == points_on_circle - 1) i2 = current_point - points_on_circle;
				else i2 = current_point + v - points_on_circle + 1;
				if (v == points_on_circle - 1) i3 = current_point;
				else i3 = current_point + v + 1;
				indices.push_back(i1);
				indices.push_back(i2);
				indices.push_back(i3);
				
			}
		}
		current_point += points_on_circle;
	}
	//Log::LogDebug(std::to_string(indices.size()));

	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof( WORD ) * indices.size();
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;
	
	InitData.pSysMem = indices.data(); 
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
	render_system->immediate_context_->DrawIndexed(  indices.size(), 0, 0 ); //indices.size()
	//render_system->immediate_context_->Draw(vertices.size(), 0);

	vertex_buffer_->Release();
	index_buffer_->Release();
	constant_buffer_->Release();
	//Log::LogDebug("Drawing 0");
}

bool RenderPlanet::Release() {
	return true;
}