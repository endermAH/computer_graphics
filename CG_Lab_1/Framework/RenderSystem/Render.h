#pragma once

#include "../stdafx.h"

class Render
{
public:
	Render();
	virtual ~Render();

	bool CreateDevice(HWND hwnd);
	void BeginFrame();
	void EndFrame();
	void Shutdown();

	virtual bool Init(HWND hwnd) = 0;
	virtual bool Draw() = 0;
	virtual void Close() = 0;

protected:
	HRESULT CompileShaderFromFile(std::string FileName, LPCSTR EntryPoint, LPCSTR ShaderModel, ID3DBlob** ppBlobOut);
	void CheckCompileResult(HRESULT res, ID3DBlob* error_code);

public:
	D3D_DRIVER_TYPE driver_type_;
	D3D_FEATURE_LEVEL feature_level_;
	
	ID3D11Device *device_;
	ID3D11DeviceContext *immediate_context_;
	IDXGISwapChain *swap_chain_;
	ID3D11RenderTargetView *render_target_view_;

	ID3D11Texture2D *m_pDepthStencil;
	ID3D11DepthStencilView *m_pDepthStencilView;
};
