#pragma once

#include "stdafx.h"
#include "Render.h"

class RenderTriangle : public Render
{
public:
	RenderTriangle();
	bool Init(HWND hwnd);
	bool Draw();
	void Close();

protected:
	ID3D11Buffer *m_pVertexBuffer;
	ID3D11InputLayout *m_pVertexLayout;	
	ID3D11VertexShader *m_pVertexShader;
	ID3D11PixelShader *m_pPixelShader;
};