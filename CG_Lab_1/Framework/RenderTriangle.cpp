#include "RenderTriangle.h"
#include "Log.h"

#include <string>
#include <d3d.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <directxmath.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "dxguid.lib")

struct SimpleVertex
{
	DirectX::XMFLOAT3 Pos;
};

RenderTriangle::RenderTriangle()
{
	m_pVertexShader = nullptr;
	m_pPixelShader = nullptr;
	m_pVertexLayout = nullptr;
	m_pVertexBuffer = nullptr;
}

bool RenderTriangle::Init(HWND hwnd)
{
	HRESULT hr = S_OK;
	ID3DBlob* pVSBlob = NULL;
	
	hr = CompileShaderFromFile("default.hlsl", "VS", "vs_5_0", &pVSBlob );
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
			0 },
	}; //change
	
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
	hr = CompileShaderFromFile( "default.hlsl", "PS", "ps_5_0", &pPSBlob );
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
		DirectX::XMFLOAT3( 0.0f, 0.5f, 0.5f ),
		DirectX::XMFLOAT3( 0.5f, -0.5f, 0.5f ),
		DirectX::XMFLOAT3( -0.5f, -0.5f, 0.5f ),
	};

	D3D11_BUFFER_DESC bd;
	ZeroMemory( &bd, sizeof(bd) );
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof( SimpleVertex ) * 3;
	//bd.ByteWidth = sizeof( DirectX::XMFLOAT4 ) * 6;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA Data;
	ZeroMemory( &Data, sizeof(Data) );
	Data.pSysMem = vertices;

	hr = device_->CreateBuffer(&bd, &Data, &m_pVertexBuffer);
	if( FAILED( hr ) )
		return false;
	
	UINT stride = sizeof( SimpleVertex );
	//UINT stride[] = { 32 };
	UINT offset = 0;
	immediate_context_->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);
	
	immediate_context_->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	
	return true;
}

bool RenderTriangle::Draw()
{
	immediate_context_->VSSetShader( m_pVertexShader, NULL, 0 );
	immediate_context_->PSSetShader( m_pPixelShader, NULL, 0 );
	 immediate_context_->Draw( 3, 0 );
	return true;
}

void RenderTriangle::Close()
{
	m_pVertexBuffer->Release();
	m_pVertexLayout->Release();
	m_pVertexShader->Release();
	m_pPixelShader->Release();
}