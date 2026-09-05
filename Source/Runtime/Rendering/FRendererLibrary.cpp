#include "FRenderResourceLibrary.h"

#include "Runtime/Core/TArray.h"
#include "Runtime/Rendering/FRenderer.h"
#include <Windows.h>
#include <filesystem>

namespace
{
	FWString GetExecutableDirectory()
	{
		wchar_t Buffer[256];
		GetModuleFileNameW(nullptr, Buffer, 256);
		return std::filesystem::path(Buffer).parent_path();
	}
}

bool FRenderResourceLibrary::Initialize(FRenderer& Renderer)
{
	if (!CreateCubeMesh(Renderer) ||
		!CreateSimpleMaterial(Renderer))
	{
		return false;
	}

	return true;
}

bool FRenderResourceLibrary::CreateCubeMesh(FRenderer& Renderer)
{

	TArray<FVertexPositionColor> Vertices = {
		{ FVector3f(-0.5, -0.5, -0.5), FVector3f(0.0, 0.0, 0.0) },
		{ FVector3f(0.5, -0.5, -0.5), FVector3f(1.0, 0.0, 0.0) },
		{ FVector3f(0.5,  0.5, -0.5), FVector3f(1.0, 1.0, 0.0) },
		{ FVector3f(-0.5,  0.5, -0.5), FVector3f(0.0, 1.0, 0.0) },
		{ FVector3f(-0.5, -0.5,  0.5), FVector3f(0.0, 0.0, 1.0) },
		{ FVector3f(0.5, -0.5,  0.5), FVector3f(1.0, 0.0, 1.0) },
		{ FVector3f(0.5,  0.5,  0.5), FVector3f(1.0, 1.0, 1.0) },
		{ FVector3f(-0.5,  0.5,  0.5), FVector3f(0.0, 1.0, 1.0) },
	};

	const TArray<uint32> Indices = {
		0, 2, 1, 0, 3, 2, // -Z
		4, 5, 6, 4, 6, 7, // +Z
		0, 1, 5, 0, 5, 4, // -Y
		3, 7, 6, 3, 6, 2, // +Y
		0, 4, 7, 0, 7, 3, // -X
		1, 2, 6, 1, 6, 5, // +X
	};

	FMeshDesc MeshDesc{
		.VertexLayout = EVertexLayout::PositionColor,
		.VertexData = Vertices.data(),
		.VertexDataSize = static_cast<uint32>(sizeof(FVertexPositionColor) * Vertices.size()),
		.VertexStride = static_cast<uint32>(sizeof(FVertexPositionColor)),
		.VertexCount = static_cast<uint32>(Vertices.size()),
		.IndexData = Indices.data(),
		.IndexDataSize = static_cast<uint32>(sizeof(uint32) * Indices.size()),
		.IndexCount = static_cast<uint32>(Indices.size()),
	};

	CubeMesh = Renderer.CreateMesh(MeshDesc);

	return CubeMesh != nullptr;
}

bool FRenderResourceLibrary::CreateSimpleMaterial(FRenderer& Renderer)
{
	FWString Path = GetExecutableDirectory();

	FMaterialDesc Desc = {
		.VertexShaderFileName = Path + L"/Shader/ExampleVS.cso",
		.PixelShaderFileName = Path + L"/Shader/ExamplePS.cso",
		.VertexLayout = EVertexLayout::PositionColor,
	};
	
	SimpleMaterial = Renderer.CreateMaterial(Desc);

	return SimpleMaterial != nullptr;
}
