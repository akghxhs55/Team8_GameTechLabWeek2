#pragma once

#include "FMesh.h"
#include "FMaterial.h"
#include "FRenderPipeline.h"
#include "ShaderConstants.h"
#include "Runtime/Core/PointerTypes.h"
#include <Windows.h>
#include <d3d11.h>
#include <wrl/client.h>

class FRenderer final
{
public:
	bool Initialize(HWND Window);
	void Shutdown();
	void BeginFrame();
	void Draw(const FMesh& Mesh, const FMaterial& Material, const FObjectConstants& ObjectConstants);
	void SwapBuffer();
	
	[[nodiscard]]
	TSharedPtr<FMesh> CreateMesh(const FMeshDesc& Desc);
	[[nodiscard]]
	TSharedPtr<FMaterial> CreateMaterial(const FMaterialDesc& Desc);

	void GetDeviceAndContext_ImplDX11(ID3D11Device*& DeviceOut, ID3D11DeviceContext*& ContextOut);
private:
	bool InitializeDeviceAndSwapChain(HWND Window);
	bool InitializeBackBufferAndDepthStencil();
	bool InitializeConstantBuffers();
	void UpdateObjectConstants(const FObjectConstants& Constants);

	[[nodiscard]]
	TSharedPtr<FRenderPipeline> FindOrCreateRenderPipeline(const FMaterialDesc& Desc);

private:
	Microsoft::WRL::ComPtr<ID3D11Device> Device;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> Context;
	Microsoft::WRL::ComPtr<IDXGISwapChain> SwapChain;
	D3D11_VIEWPORT Viewport{};

	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> BackBufferRTV;
	Microsoft::WRL::ComPtr<ID3D11Texture2D> DepthStencilBuffer;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> DepthStencilView;

	// TODO: FRenderer가 Constants 형태에 종속되는 상태. 필요하다면 분리할 수 있음
	Microsoft::WRL::ComPtr<ID3D11Buffer> FrameConstantBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> ObjectConstantBuffer;
};
