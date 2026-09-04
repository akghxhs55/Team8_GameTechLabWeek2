#include "FRenderer.h"

#include <Windows.h>
#include <d3d11.h>
#include <wrl/client.h>

bool FRenderer::Initialize(HWND Window)
{
	constexpr D3D_FEATURE_LEVEL FeatureLevels[] = { D3D_FEATURE_LEVEL_11_0 };

	DXGI_SWAP_CHAIN_DESC SwapChainDesc{};
	SwapChainDesc.BufferDesc.Width = 0u;
	SwapChainDesc.BufferDesc.Height = 0u;
	SwapChainDesc.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	SwapChainDesc.SampleDesc.Count = 1u;
	SwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	SwapChainDesc.BufferCount = 2u;
	SwapChainDesc.OutputWindow = Window;
	SwapChainDesc.Windowed = true;
	SwapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	UINT CreateDeviceFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
#ifndef NDEBUG
	CreateDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	HRESULT Result = D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		CreateDeviceFlags,
		FeatureLevels,
		ARRAYSIZE(FeatureLevels),
		D3D11_SDK_VERSION,
		&SwapChainDesc,
		&SwapChain,
		&Device,
		nullptr,
		&Context);
	if (FAILED(Result))
	{
		return false;
	}

	UINT Count = 1u;
	Context->RSGetViewports(&Count, &Viewport);

	return true;
}

void FRenderer::Release()
{
	if (Context)
	{
		Context->ClearState();
		Context->Flush();
	}

	SwapChain.Reset();
	Context.Reset();
	Device.Reset();
}

void FRenderer::PrepareFrame()
{
}

void FRenderer::Draw()
{
}

void FRenderer::SwapBuffer()
{
	SwapChain->Present(1u, 0u);
}
