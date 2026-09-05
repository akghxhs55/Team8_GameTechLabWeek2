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
	[[nodiscard]] TSharedPtr<FMaterial> GetSimpleMaterial() { return SimpleMaterial; }

private:
	bool CreateCubeMesh(FRenderer& Renderer);
	bool CreateCylinderMesh(FRenderer& Renderer);
	bool CreateSimpleMaterial(FRenderer& Renderer);

private:
	TSharedPtr<FMesh> CubeMesh;
	TSharedPtr<FMesh> CylinderMesh;
	TSharedPtr<FMaterial> SimpleMaterial;
};
