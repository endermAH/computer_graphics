#pragma once

#include "../Framework/stdafx.h"
#include "../Framework/RenderSystem/Render.h"

class RenderRotatingCubes : public Render
{
public:
	RenderRotatingCubes();
	bool Init(HWND hwnd);
	bool Draw();
	void Close();

	void Update();

	void* operator new(size_t i)
	{
		return _aligned_malloc(i,16);
	}

	void operator delete(void* p)
	{
		_aligned_free(p);
	}

private:		
	ID3D11Buffer *m_pVertexBuffer;
	ID3D11InputLayout *m_pVertexLayout;	
	ID3D11VertexShader *m_pVertexShader;
	ID3D11PixelShader *m_pPixelShader;

	ID3D11Buffer *m_pIndexBuffer;
	ID3D11Buffer *m_pConstantBuffer;

	DirectX::XMMATRIX m_World1;
	DirectX::XMMATRIX m_World2;
	DirectX::XMMATRIX m_View;
	DirectX::XMMATRIX m_Projection;
};
