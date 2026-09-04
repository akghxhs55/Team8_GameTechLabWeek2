#include "FRenderer.h"

#include "FMesh.h"
#include "FMaterial.h"
#include "FRenderPipeline.h"
#include "Vertices.h"
#include "Runtime/Core/Pointers.h"
#include <Windows.h>
#include <d3d11.h>
#include <wrl/client.h>
#include <d3dcompiler.h>

bool FRenderer::Initialize(HWND Window)
{
	if (!InitializeDeviceAndSwapChain((Window)) ||
		!InitializeBackBuffer())
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

	BackBufferRTV.Reset();

	SwapChain.Reset();
	Context.Reset();
	Device.Reset();
}

void FRenderer::BeginFrame()
{
	Context->RSSetViewports(1, &Viewport);
	Context->OMSetRenderTargets(1, BackBufferRTV.GetAddressOf(), nullptr);

	constexpr float ClearColor[] = { 0.05f, 0.05f, 0.08f, 1.0f };
	Context->ClearRenderTargetView(BackBufferRTV.Get(), ClearColor);
}

void FRenderer::Draw(const FMesh& Mesh, const FMaterial& Material)
{
	if (Mesh.GetVertexLayout() != Material.GetVertexLayout())
	{
		return;
	}

	const auto& Pipeline = Material.Pipeline;

	Pipeline->Bind(Context.Get());
	Material.BindResources(Context.Get());
	Mesh.BindResources(Context.Get());

	Context->OMSetBlendState(nullptr, nullptr, 0xFFFFFFFF);

	if (Mesh.HasIndices())
	{
		Context->DrawIndexed(Mesh.IndexCount, 0, 0);
	}
	else
	{
		Context->Draw(Mesh.VertexCount, 0);
	}
}

void FRenderer::SwapBuffer()
{
	SwapChain->Present(1u, 0u);
}

TSharedPtr<FMesh> FRenderer::CreateMesh(const FMeshDesc& Desc)
{
	if (!Desc.VertexData ||
		Desc.VertexCount == 0 ||
		Desc.VertexDataSize == 0 ||
		Desc.VertexStride == 0)
	{
		return nullptr;
	}
	if (Desc.IndexCount > 0 && (!Desc.IndexData || Desc.IndexDataSize == 0))
	{
		return nullptr;
	}

	auto Mesh = TSharedPtr<FMesh>{ new FMesh() };
	Mesh->VertexLayout = Desc.VertexLayout;

	D3D11_BUFFER_DESC VertexBufferDesc = {
		.ByteWidth = Desc.VertexDataSize,
		.Usage = D3D11_USAGE_DEFAULT,
		.BindFlags = D3D11_BIND_VERTEX_BUFFER,
	};

	D3D11_SUBRESOURCE_DATA VertexData = {
		.pSysMem = Desc.VertexData,
	};

	HRESULT Result = Device->CreateBuffer(&VertexBufferDesc, &VertexData, &Mesh->VertexBuffer);
	if (FAILED(Result))
	{
		return nullptr;
	}
	Mesh->VertexCount = Desc.VertexCount;
	Mesh->VertexStride = Desc.VertexStride;

	if (Desc.IndexCount > 0 && Desc.IndexData)
	{
		D3D11_BUFFER_DESC IndexBufferDesc = {
			.ByteWidth = Desc.IndexDataSize,
			.Usage = D3D11_USAGE_DEFAULT,
			.BindFlags = D3D11_BIND_INDEX_BUFFER,
		};

		D3D11_SUBRESOURCE_DATA IndexData = {
			.pSysMem = Desc.IndexData,
		};

		Result = Device->CreateBuffer(&IndexBufferDesc, &IndexData, &Mesh->IndexBuffer);
		if (FAILED(Result))
		{
			return nullptr;
		}
	}
	Mesh->IndexCount = Desc.IndexCount;

	return Mesh;
}

TSharedPtr<FMaterial> FRenderer::CreateMaterial(const FMaterialDesc& Desc)
{
	TSharedPtr<FMaterial> Material{ new FMaterial() };

	Material->Pipeline = FindOrCreateRenderPipeline(Desc);
	if (!Material->Pipeline)
	{
		return nullptr;
	}

	return Material;
}

bool FRenderer::InitializeDeviceAndSwapChain(HWND Window)
{
	constexpr D3D_FEATURE_LEVEL FeatureLevels[] = { D3D_FEATURE_LEVEL_11_0 };

	DXGI_SWAP_CHAIN_DESC SwapChainDesc{
		.BufferDesc = {
			.Width = 0u,
			.Height = 0u,
			.Format = DXGI_FORMAT_B8G8R8A8_UNORM_SRGB,
		},
		.SampleDesc = {
			.Count = 1u,
		},
		.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT,
		.BufferCount = 2u,
		.OutputWindow = Window,
		.Windowed = true,
		.SwapEffect = DXGI_SWAP_EFFECT_DISCARD,
	};

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

	Viewport = {
		.TopLeftX = 0.0f,
		.TopLeftY = 0.0f,
		.Width = static_cast<float>(ClientRect.right - ClientRect.left),
		.Height = static_cast<float>(ClientRect.bottom - ClientRect.top),
		.MinDepth = 0.0f,
		.MaxDepth = 1.0f,
	};

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

TSharedPtr<FRenderPipeline> FRenderer::FindOrCreateRenderPipeline(const FMaterialDesc& Desc)
{
	// TODO: 이미 만들었던 파이프라인 있는지 찾아서 쓰기

	TSharedPtr<FRenderPipeline> Pipeline{ new FRenderPipeline() };
	Pipeline->VertexLayout = Desc.VertexLayout;

	Microsoft::WRL::ComPtr<ID3DBlob> Blob;
	HRESULT Result = D3DReadFileToBlob(Desc.VertexShaderFileName.c_str(), &Blob);
	if (FAILED(Result))
	{
		return nullptr;
	}

	Result = Device->CreateVertexShader(
		Blob->GetBufferPointer(),
		Blob->GetBufferSize(),
		nullptr,
		&Pipeline->VertexShader);
	if (FAILED(Result))
	{
		return nullptr;
	}

	FVertexLayoutDesc LayoutDesc = GetVertexLayoutDesc(Desc.VertexLayout);
	Result = Device->CreateInputLayout(LayoutDesc.InputElements, LayoutDesc.InputElementCount, Blob->GetBufferPointer(), Blob->GetBufferSize(), &Pipeline->InputLayout);
	if (FAILED(Result))
	{
		return nullptr;
	}

	Result = D3DReadFileToBlob(Desc.PixelShaderFileName.c_str(), &Blob);
	if (FAILED(Result))
	{
		return nullptr;
	}

	Result = Device->CreatePixelShader(
		Blob->GetBufferPointer(),
		Blob->GetBufferSize(),
		nullptr,
		&Pipeline->PixelShader);
	if (FAILED(Result))
	{
		return nullptr;
	}

	D3D11_RASTERIZER_DESC RasterizerDesc{
		.FillMode = D3D11_FILL_SOLID,
		.CullMode = D3D11_CULL_NONE,
	};

	Result = Device->CreateRasterizerState(&RasterizerDesc, &Pipeline->RasterizerState);
	if (FAILED(Result))
	{
		return nullptr;
	}

	return Pipeline;
}
