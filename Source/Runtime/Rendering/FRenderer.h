#pragma once

#include "FMesh.h"
#include "FMaterial.h"
#include "FRenderPipeline.h"
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
	void Draw(const FMesh& Mesh, const FMaterial& Material);
	void SwapBuffer();
	
	[[nodiscard]]
	TSharedPtr<FMesh> CreateMesh(const FMeshDesc& Desc);
	[[nodiscard]]
	TSharedPtr<FMaterial> CreateMaterial(const FMaterialDesc& Desc);

private:
	bool InitializeDeviceAndSwapChain(HWND Window);
	bool InitializeBackBuffer();

	[[nodiscard]]
	TSharedPtr<FRenderPipeline> FindOrCreateRenderPipeline(const FMaterialDesc& Desc);

private:
	Microsoft::WRL::ComPtr<ID3D11Device> Device;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> Context;
	Microsoft::WRL::ComPtr<IDXGISwapChain> SwapChain;
	D3D11_VIEWPORT Viewport{};

	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> BackBufferRTV;
};
