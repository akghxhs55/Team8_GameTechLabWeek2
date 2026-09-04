#include "FRenderer.h"

#include <Windows.h>
#include <d3d11.h>
#include <wrl/client.h>
#include <d3dcompiler.h>

bool FRenderer::Initialize(HWND Window)
{
	if (!InitializeDeviceAndSwapChain((Window)) ||
		!InitializeBackBuffer() ||
		!InitializeShaderAndRasterizer())
	{
		Shutdown();
		return false;
	}

	return true;
}

void FRenderer::Shutdown()
{
	if (Context)
	{
		Context->ClearState();
		Context->Flush();
	}

	VertexShader.Reset();
	PixelShader.Reset();
	RasterizerState.Reset();

	BackBufferRTV.Reset();

	SwapChain.Reset();
	Context.Reset();
	Device.Reset();
}

void FRenderer::BeginFrame()
{
	constexpr float ClearColor[] = { 0.05f, 0.05f, 0.08f, 1.0f };
	Context->ClearRenderTargetView(BackBufferRTV.Get(), ClearColor);
}

void FRenderer::Draw()
{
	Context->IASetInputLayout(nullptr);
	Context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	Context->VSSetShader(VertexShader.Get(), nullptr, 0);
	Context->PSSetShader(PixelShader.Get(), nullptr, 0);

	Context->RSSetViewports(1, &Viewport);
	Context->RSSetState(RasterizerState.Get());

	Context->OMSetRenderTargets(1, BackBufferRTV.GetAddressOf(), nullptr);
	Context->OMSetBlendState(nullptr, nullptr, 0xFFFFFFFF);

	Context->Draw(3, 0);
}

void FRenderer::SwapBuffer()
{
	SwapChain->Present(1u, 0u);
}

bool FRenderer::InitializeDeviceAndSwapChain(HWND Window)
{
	constexpr D3D_FEATURE_LEVEL FeatureLevels[] = { D3D_FEATURE_LEVEL_11_0 };

	DXGI_SWAP_CHAIN_DESC SwapChainDesc{};
	SwapChainDesc.BufferDesc.Width = 0u;
	SwapChainDesc.BufferDesc.Height = 0u;
	SwapChainDesc.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM_SRGB;
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

	RECT ClientRect{};
	GetClientRect(Window, &ClientRect);

	Viewport.TopLeftX = 0.0f;
	Viewport.TopLeftY = 0.0f;
	Viewport.Width = static_cast<float>(ClientRect.right - ClientRect.left);
	Viewport.Height = static_cast<float>(ClientRect.bottom - ClientRect.top);
	Viewport.MinDepth = 0.0f;
	Viewport.MaxDepth = 1.0f;

	return true;
}

bool FRenderer::InitializeBackBuffer()
{
	Microsoft::WRL::ComPtr<ID3D11Texture2D> BackBuffer;
	HRESULT Result = SwapChain->GetBuffer(0, IID_PPV_ARGS(&BackBuffer));
	if (FAILED(Result))
	{
		return false;
	}

	Result = Device->CreateRenderTargetView(BackBuffer.Get(), nullptr, &BackBufferRTV);
	if (FAILED(Result))
	{
		return false;
	}

	return true;
}

bool FRenderer::InitializeShaderAndRasterizer()
{
	Microsoft::WRL::ComPtr<ID3DBlob> Blob;
	HRESULT Result = D3DReadFileToBlob(L"Shader/ExampleVS.cso", &Blob);
	if (FAILED(Result))
	{
		return false;
	}

	Result = Device->CreateVertexShader(
		Blob->GetBufferPointer(),
		Blob->GetBufferSize(),
		nullptr,
		&VertexShader);
	if (FAILED(Result))
	{
		return false;
	}

	Result = D3DReadFileToBlob(L"Shader/ExamplePS.cso", &Blob);
	if (FAILED(Result))
	{
		return false;
	}

	Result = Device->CreatePixelShader(
		Blob->GetBufferPointer(),
		Blob->GetBufferSize(),
		nullptr,
		&PixelShader);
	if (FAILED(Result))
	{
		return false;
	}

	D3D11_RASTERIZER_DESC RasterizerDesc{};
	RasterizerDesc.FillMode = D3D11_FILL_SOLID;
	RasterizerDesc.CullMode = D3D11_CULL_BACK;

	Result = Device->CreateRasterizerState(&RasterizerDesc, &RasterizerState);
	if (FAILED(Result))
	{
		return false;
	}

	return true;
}
