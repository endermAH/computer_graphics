#include "RenderCube.h"

#include "RenderCube.h"
#include <d3d.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <directxmath.h>

#include "Log.h"

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "dxguid.lib")

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

RenderCube::RenderCube()
{
	m_pVertexShader = nullptr;
	m_pPixelShader = nullptr;
	m_pVertexLayout = nullptr;
	m_pVertexBuffer = nullptr;

	m_pIndexBuffer = nullptr;
	m_pConstantBuffer = nullptr;
}

bool RenderCube::Init(HWND hwnd)
{
	HRESULT hr = S_OK;
	ID3DBlob* pVSBlob = NULL;
	
	hr = CompileShaderFromFile("cube_shader.hlsl", "VS", "vs_5_0", &pVSBlob );
	if(FAILED(hr)) return false;

	hr = device_->CreateVertexShader( pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), NULL, &m_pVertexShader );
	if( FAILED( hr ) )
	{	
		Log::LogError("Can not create vertex shader");
		pVSBlob->Release();
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
	
	UINT numElements = ARRAYSIZE( layout );
	
	hr = device_->CreateInputLayout(
		layout,
		numElements,
		pVSBlob->GetBufferPointer(),
		pVSBlob->GetBufferSize(),
		&m_pVertexLayout);

	pVSBlob->Release();
	if( FAILED( hr ) ) {
		Log::LogError("Can not create a layout");
		return false;
	}

	immediate_context_->IASetInputLayout( m_pVertexLayout );

	ID3DBlob* pPSBlob = NULL;
	hr = CompileShaderFromFile( "cube_shader.hlsl", "PS", "ps_5_0", &pPSBlob );
	if(FAILED(hr)) return false;
	
	hr = device_->CreatePixelShader( pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), NULL, &m_pPixelShader );
	pPSBlob->Release();
	if( FAILED( hr ) )
	{	
		Log::LogError("Can not create pixel shader");
		pPSBlob->Release();
		return false;
	}

	SimpleVertex vertices[] =
	{
		{ DirectX::XMFLOAT3( -1.0f, 1.0f, -1.0f ), DirectX::XMFLOAT4( 0.0f, 0.0f, 1.0f, 1.0f ) },
		{ DirectX::XMFLOAT3( 1.0f, 1.0f, -1.0f ), DirectX::XMFLOAT4( 0.0f, 1.0f, 0.0f, 1.0f ) },
		{ DirectX::XMFLOAT3( 1.0f, 1.0f, 1.0f ), DirectX::XMFLOAT4( 1.0f, 0.0f, 0.0f, 1.0f ) },
		{ DirectX::XMFLOAT3( -1.0f, 1.0f, 1.0f ), DirectX::XMFLOAT4( 1.0f, 0.0f, 0.0f, 1.0f ) },
		{ DirectX::XMFLOAT3( -1.0f, -1.0f, -1.0f ), DirectX::XMFLOAT4( 1.0f, 0.0f, 1.0f, 1.0f ) },
		{ DirectX::XMFLOAT3( 1.0f, -1.0f, -1.0f ), DirectX::XMFLOAT4( 1.0f, 1.0f, 0.0f, 1.0f ) },
		{ DirectX::XMFLOAT3( 1.0f, -1.0f, 1.0f ), DirectX::XMFLOAT4( 1.0f, 1.0f, 1.0f, 1.0f ) },
		{ DirectX::XMFLOAT3( -1.0f, -1.0f, 1.0f ), DirectX::XMFLOAT4( 0.0f, 0.0f, 0.0f, 1.0f ) }
	};

	D3D11_BUFFER_DESC bd;
	ZeroMemory( &bd, sizeof(bd) );
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof( SimpleVertex ) * 8;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	
	D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory( &InitData, sizeof(InitData) );
	InitData.pSysMem = vertices;
	hr = device_->CreateBuffer(&bd, &InitData, &m_pVertexBuffer);
	if( FAILED( hr ) )
		return false;

	UINT stride = sizeof( SimpleVertex );
	UINT offset = 0;
	immediate_context_->IASetVertexBuffers( 0, 1, &m_pVertexBuffer, &stride, &offset);

	WORD indices[] =
	{
		3,1,0,
		2,1,3,

		0,5,4,
		1,5,0,

		3,4,7,
		0,4,3,

		1,6,5,
		2,6,1,

		2,7,6,
		3,7,2,

		6,4,5,
		7,4,6,
	};

	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof( WORD ) * 36;
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;
	
	InitData.pSysMem = indices;
	hr = device_->CreateBuffer( &bd, &InitData, &m_pIndexBuffer );
	if(FAILED(hr)) return false;

	immediate_context_->IASetIndexBuffer( m_pIndexBuffer, DXGI_FORMAT_R16_UINT, 0 );

	immediate_context_->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(ConstantBuffer);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
	hr = device_->CreateBuffer( &bd, NULL, &m_pConstantBuffer );
	if(FAILED(hr)) return false;

	m_World = DirectX::XMMatrixIdentity();
	
	DirectX::XMVECTOR Eye = DirectX::XMVectorSet( 0.0f, 1.0f, -5.0f, 0.0f );
	DirectX::XMVECTOR At = DirectX::XMVectorSet( 0.0f, 1.0f, 0.0f, 0.0f );
	DirectX::XMVECTOR Up = DirectX::XMVectorSet( 0.0f, 1.0f, 0.0f, 0.0f );
	m_View = DirectX::XMMatrixLookAtLH( Eye, At, Up );

	float width = 640.0f;
	float height = 480.0f;
	m_Projection = DirectX::XMMatrixPerspectiveFovLH(DirectX::XM_PIDIV2, width/height, 0.01f, 100.0f );
	
	return true;
}

bool RenderCube::Draw()
{
	static float t = 0.0f;
	static DWORD dwTimeStart = 0;
	DWORD dwTimeCur = GetTickCount();
	if( dwTimeStart == 0 )
		dwTimeStart = dwTimeCur;
	t = ( dwTimeCur - dwTimeStart ) / 1000.0f;

	m_World = DirectX::XMMatrixRotationY( -t );

	ConstantBuffer cb;
	cb.mWorld = XMMatrixTranspose( m_World );
	cb.mView = XMMatrixTranspose( m_View );
	cb.mProjection = XMMatrixTranspose( m_Projection );
	immediate_context_->UpdateSubresource( m_pConstantBuffer, 0, NULL, &cb, 0, 0 );

	immediate_context_->VSSetShader( m_pVertexShader, NULL, 0 );
	immediate_context_->VSSetConstantBuffers( 0, 1, &m_pConstantBuffer );
	immediate_context_->PSSetShader( m_pPixelShader, NULL, 0 );
	immediate_context_->DrawIndexed( 36, 0, 0 );

	return true;
}

void RenderCube::Close()
{
	m_pVertexBuffer->Release();
	m_pVertexLayout->Release();
	m_pVertexShader->Release();
	m_pPixelShader->Release();
	m_pVertexShader->Release();
	m_pPixelShader->Release();
}