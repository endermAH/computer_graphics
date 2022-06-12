#include "TriangleComponent.h"

TriangleComponent::TriangleComponent(Game* game, DirectX::XMFLOAT4 points[6]) {
	points_ = points;
	game_ = game;
	Init();
}

void TriangleComponent::Init() {
	CompileShaders();
	CreateLayout();
	Draw();
}

void TriangleComponent::Update() {
	game_->vb[game_->vb_count_] = vertex_buffer_;
	game_->vb_count_++;
	//game_->context_->IASetIndexBuffer(index_buffer_, DXGI_FORMAT_R32_UINT, 0);
};

void TriangleComponent::CompileShaders() {
	ID3DBlob* errorVertexCode = nullptr;
	auto res = D3DCompileFromFile(L"../Shaders/default.hlsl",
		nullptr /*macros*/,
		nullptr /*include*/,
		"VSMain",
		"vs_5_0",
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
		0,
		&vertexBC_,
		&errorVertexCode);

	CheckCompileResult(res, errorVertexCode);

	D3D_SHADER_MACRO Shader_Macros[] = { "TEST", "1", "TCOLOR", "float4(0.0f, 1.0f, 0.0f, 1.0f)", nullptr, nullptr };

	ID3DBlob* pixelBC;
	ID3DBlob* errorPixelCode;
	res = D3DCompileFromFile(L"../Shaders/default.hlsl",
		Shader_Macros /*macros*/,
		nullptr /*include*/,
		"PSMain",
		"ps_5_0",
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
		0,
		&pixelBC,
		&errorPixelCode);

	CheckCompileResult(res, errorVertexCode);
	

	game_->device_->CreateVertexShader(
		vertexBC_->GetBufferPointer(),
		vertexBC_->GetBufferSize(),
		nullptr, &vertex_shader_);

	game_->device_->CreatePixelShader(
		pixelBC->GetBufferPointer(),
		pixelBC->GetBufferSize(),
		nullptr, &pixel_shader_);

	game_->context_->VSSetShader(vertex_shader_, nullptr, 0);
	game_->context_->PSSetShader(pixel_shader_, nullptr, 0);
	
}

void TriangleComponent::CheckCompileResult(HRESULT res, ID3DBlob* error_code) {
	if (FAILED(res)) {
		// If the shader failed to compile it should have written something to the error message.
		if (error_code) {
			char* compileErrors = (char*)(error_code->GetBufferPointer());

			std::cout << compileErrors << std::endl;
		}
		// If there was  nothing in the error message then it simply could not find the shader file itself.
		else
		{
			std::cout << "Missing shader file" << std::endl;
		}
	}
}

void TriangleComponent::CreateLayout() {
	D3D11_INPUT_ELEMENT_DESC inputElements[] = {
		D3D11_INPUT_ELEMENT_DESC {
			"POSITION",
			0,
			DXGI_FORMAT_R32G32B32A32_FLOAT,
			0,
			0,
			D3D11_INPUT_PER_VERTEX_DATA,
			0},
		D3D11_INPUT_ELEMENT_DESC {
			"COLOR",
			0,
			DXGI_FORMAT_R32G32B32A32_FLOAT,
			0,
			D3D11_APPEND_ALIGNED_ELEMENT,
			D3D11_INPUT_PER_VERTEX_DATA,
			0}
	};

	game_->device_->CreateInputLayout(
		inputElements,
		2,
		vertexBC_->GetBufferPointer(),
		vertexBC_->GetBufferSize(),
		&layout_);

	game_->context_->IASetInputLayout(layout_);
}

void TriangleComponent::Draw() {
	D3D11_BUFFER_DESC vertexBufDesc = {};
	vertexBufDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufDesc.CPUAccessFlags = 0;
	vertexBufDesc.MiscFlags = 0;
	vertexBufDesc.StructureByteStride = 0;
	vertexBufDesc.ByteWidth = sizeof(DirectX::XMFLOAT4) * 6;

	D3D11_SUBRESOURCE_DATA vertexData = {};
	vertexData.pSysMem = points_;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	game_->device_->CreateBuffer(&vertexBufDesc, &vertexData, &vertex_buffer_);

	int indeces[] = { 0,1,2 };
	D3D11_BUFFER_DESC indexBufDesc = {};
	indexBufDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufDesc.CPUAccessFlags = 0;
	indexBufDesc.MiscFlags = 0;
	indexBufDesc.StructureByteStride = 0;
	indexBufDesc.ByteWidth = sizeof(int) * std::size(indeces);

	D3D11_SUBRESOURCE_DATA indexData = {};
	indexData.pSysMem = indeces;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	game_->device_->CreateBuffer(&indexBufDesc, &indexData, &index_buffer_);

}