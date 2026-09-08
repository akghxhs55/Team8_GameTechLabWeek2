#pragma once

#include "FMesh.h"
#include "FMaterial.h"
#include "Runtime/Core/PointerTypes.h"

class FRenderer;

class FRenderResourceLibrary final
{
public:
	bool Initialize(FRenderer& Renderer);

	// TODO: 필요할 시 Enum 기반 switch-case로 처리
	//  GetConstCubeMesh() -> const FMesh를 반환, GetCubeMesh() -> FMesh를 생성해서 반환으로 나눌 수 있음
	//  지금은 어차피 FMesh로 반환해도 Getter만 있어서 수정 못 함
	//  UPrimitive가 비const FMesh를 소유하도록 되어 있어서 이렇게 둠
	[[nodiscard]] TSharedPtr<FMesh> GetCubeMesh() { return CubeMesh; }
	[[nodiscard]] TSharedPtr<FMesh> GetCylinderMesh() { return CylinderMesh; }
	[[nodiscard]] TSharedPtr<FMesh> GetConeMesh() { return ConeMesh; }
	[[nodiscard]] TSharedPtr<FMesh> GetArrowMesh() { return ArrowMesh; }
	[[nodiscard]] TSharedPtr<FMesh> GetGridMesh() { return GridMesh; }
	[[nodiscard]] TSharedPtr<FMaterial> GetSimpleMaterial() { return SimpleMaterial; }
	[[nodiscard]] TSharedPtr<FMaterial> GetGridMaterial() { return GridMaterial; }
private:
	bool CreateCubeMesh(FRenderer& Renderer);
	bool CreateCylinderMesh(FRenderer& Renderer, float Height, uint32 SliceCount, float TopRadius, float BottomRadius);
	bool CreateConeMesh(FRenderer& Renderer);
	bool CreateArrowMesh(FRenderer& Renderer);
	bool CreateGridMesh(FRenderer& Renderer);
	bool CreateSimpleMaterial(FRenderer& Renderer);
	bool CreateGridMaterial(FRenderer& Renderer);

private:
	TSharedPtr<FMesh> CubeMesh;
	TSharedPtr<FMesh> CylinderMesh;
	TSharedPtr<FMesh> ConeMesh;
	TSharedPtr<FMesh> ArrowMesh;
	TSharedPtr<FMesh> GridMesh;
	TSharedPtr<FMaterial> SimpleMaterial;
	TSharedPtr<FMaterial> GridMaterial;
};
