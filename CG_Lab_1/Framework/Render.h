#pragma once

#include <Windows.h>
#include <d3d.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <directxmath.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "dxguid.lib")

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
	HRESULT CompileShaderFromFile(std::string FileName, LPCSTR EntryPoint, LPCSTR ShaderModel, ID3DBlob** ppBlobOut );
	void CheckCompileResult(HRESULT res, ID3DBlob* error_code);

protected:
	D3D_DRIVER_TYPE driver_type_;
	D3D_FEATURE_LEVEL feature_level_;
	
	ID3D11Device *device_;
	ID3D11DeviceContext *immediate_context_;
	IDXGISwapChain *swap_chain_;
	ID3D11RenderTargetView *render_target_view_;
};
