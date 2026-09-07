#include "FRenderResourceLibrary.h"

#include "Runtime/Core/TArray.h"
#include "Runtime/Rendering/FRenderer.h"
#include "Runtime/Math/FVector.h"
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
		!CreateCylinderMesh(Renderer, 0,1.0f,24,1.0f,1.0f) ||
		!CreateCylinderMesh(Renderer, 1, 1.0f, 24, 1.0f, 1.0f) ||
		!CreateCylinderMesh(Renderer, 2, 1.0f, 24, 1.0f, 1.0f) ||
		!CreateConeMesh(Renderer) ||
		!CreateSimpleMaterial(Renderer))
	{
		return false;
	}

	return true;
}

bool FRenderResourceLibrary::CreateCubeMesh(FRenderer& Renderer)
{
	TArray<FVertexPositionColor> Vertices = {
		{ FVector(-0.5, -0.5, -0.5), FVector(1.0, 0.0, 0.0) },
		{ FVector(0.5, -0.5, -0.5), FVector(1.0, 0.0, 0.0) },
		{ FVector(0.5,  0.5, -0.5), FVector(1.0, 0.0, 0.0) },
		{ FVector(-0.5,  0.5, -0.5), FVector(1.0, 0.0, 0.0) },
		{ FVector(-0.5, -0.5,  0.5), FVector(0.0, 0.0, 1.0) },
		{ FVector(0.5, -0.5,  0.5), FVector(1.0, 0.0, 1.0) },
		{ FVector(0.5,  0.5,  0.5), FVector(1.0, 1.0, 1.0) },
		{ FVector(-0.5,  0.5,  0.5), FVector(0.0, 1.0, 1.0) },
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

// TODO: 컬러는 테스트용
bool FRenderResourceLibrary::CreateCylinderMesh(FRenderer& Renderer, int Axis, float Height, uint32 SliceCount, float TopRadius, float BottomRadius)
{
	constexpr float TAU = std::numbers::pi_v<float> * 2.0f;
	const float DTheta = TAU / static_cast<float>(SliceCount);

	FVector Color;
	if (Axis == 0){Color = FVector(1.0f, 0.0f, 0.0f);}
	else if (Axis == 1){Color = FVector(0.0f, 1.0f, 0.0f);}
	else{Color = FVector(0.0f, 0.0f, 1.0f);}

	TArray<FVertexPositionColor> Vertices;

	for (int Ring = 0; Ring < 2; ++Ring)
	{
		const float Radius = (Ring == 0) ? BottomRadius : TopRadius;
		const float Y = (Ring == 0) ? -0.5f * Height : 0.5f * Height;
		for (uint32 i = 0u; i <= SliceCount; ++i)
		{
			const float Theta = DTheta * static_cast<float>(i);
			const float c = cosf(Theta);
			const float s = sinf(Theta);
			const FVector Pos(Radius * c, Y, Radius * s);
			const float t = static_cast<float>(i) / static_cast<float>(SliceCount);
			//const FVector Color(t, static_cast<float>(Ring), 1.0f - t);
			Vertices.push_back({ Pos, Color });
		}
	}

	TArray<uint32> Indices;
	Indices.reserve(static_cast<uint64>(SliceCount * 6u));

	for (uint32 i = 0u; i < SliceCount; ++i)
	{
		Indices.push_back(i);
		Indices.push_back(i + SliceCount + 1);
		Indices.push_back(i + 1);

		Indices.push_back(i + 1);
		Indices.push_back(i + SliceCount + 1);
		Indices.push_back(i + SliceCount + 2);
	}

	// 위 뚜껑
	{
		//중심
		const uint32 Center = static_cast<uint32>(Vertices.size());
		Vertices.push_back({ FVector(0.0f, 0.5f * Height, 0.0f), Color });

		const uint32 First = static_cast<uint32>(Vertices.size());
		for (uint32 i = 0u; i <= SliceCount; ++i)
		{
			const float Theta = DTheta * static_cast<float>(i);
			Vertices.push_back({
				FVector(TopRadius * cosf(Theta), 0.5f * Height, TopRadius * sinf(Theta)),
				//FVector(0.9f, 0.9f, 0.2f) });
				Color });
		}

		for (uint32 i = 0u; i < SliceCount; ++i)
		{
			Indices.push_back(Center);
			Indices.push_back(First + i + 1);
			Indices.push_back(First + i);
		}
	}

	// 아래 뚜껑
	{
		const uint32 Center = static_cast<uint32>(Vertices.size());
		Vertices.push_back({ FVector(0.0f, -0.5f * Height, 0.0f), Color });

		const uint32 First = static_cast<uint32>(Vertices.size());
		for (uint32 i = 0u; i <= SliceCount; ++i)
		{
			const float Theta = DTheta * static_cast<float>(i);
			Vertices.push_back({
				FVector(BottomRadius * cosf(Theta), -0.5f * Height, BottomRadius * sinf(Theta)),
				//FVector(0.2f, 0.2f, 0.5f) });
				Color });
		}

		for (uint32 i = 0u; i < SliceCount; ++i)
		{
			Indices.push_back(Center);
			Indices.push_back(First + i);
			Indices.push_back(First + i + 1);
		}
	}

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

	CylinderMesh[Axis] = Renderer.CreateMesh(MeshDesc);
	return CylinderMesh[Axis] != nullptr;
}

bool FRenderResourceLibrary::CreateConeMesh(FRenderer& Renderer)
{
	constexpr float BottomRadius = 0.5f;
	constexpr float Height = 1.0f;
	constexpr uint32 SliceCount = 24;
	constexpr float TAU = std::numbers::pi_v<float> * 2.0f;
	constexpr float DTheta = TAU / static_cast<float>(SliceCount);

	FVector Color;
	TArray<FVertexPositionColor> Vertices;
	const float Radius = BottomRadius;

		for (int Ring = 0; Ring < 2; ++Ring)
		{

			for (uint32 i = 0u; i <= SliceCount; ++i)
			{
				const float t = static_cast<float>(i) / static_cast<float>(SliceCount);
				const FVector Color(t, static_cast<float>(i), 1.0f - t);
				if(Ring == 0)
				{
					const FVector Pos(0.0f, 0.5f, 0.0f);
					Vertices.push_back({ Pos,Color });
					continue;
				}
					
				const float Theta = DTheta * static_cast<float>(i);
				const float c = cosf(Theta);
				const float s = sinf(Theta);

				const FVector Pos(Radius * c, -0.5f, Radius * s);

				Vertices.push_back({ Pos, Color });
			}
		}


	

	TArray<uint32> Indices;
	Indices.reserve(SliceCount * 6);

	for (uint32 i = 0u; i < SliceCount; ++i)
	{
		Indices.push_back(i);
		Indices.push_back(i + SliceCount + 1);
		Indices.push_back(i + 1);

		Indices.push_back(i + 1);
		Indices.push_back(i + SliceCount + 1);
		Indices.push_back(i + SliceCount + 2);
	}

	// 아래 뚜껑
	{
		const uint32 Center = static_cast<uint32>(Vertices.size());
		Vertices.push_back({ FVector(0.0f, -0.5f * Height, 0.0f), Color });
		const uint32 First = static_cast<uint32>(Vertices.size());
		for (int i = 0; i <= SliceCount; ++i)
		{
			const float Theta = DTheta * static_cast<float>(i);
			Vertices.push_back({
				FVector(BottomRadius * cosf(Theta), -0.5f * Height, BottomRadius * sinf(Theta)),
				//FVector(0.2f, 0.2f, 0.5f) });
				Color });
		}

		for (int i = 0; i < SliceCount; ++i)
		{
			Indices.push_back(Center);
			Indices.push_back(i + First);
			Indices.push_back(i + First + 1);
		}
	}

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

	ConeMesh = Renderer.CreateMesh(MeshDesc);
	return ConeMesh != nullptr;
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
