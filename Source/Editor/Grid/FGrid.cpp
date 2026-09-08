#include "FGrid.h"

#include "Runtime/Core/IntTypes.h"
#include "Runtime/CoreUObject/USceneComponent.h"
#include "Runtime/Engine/FCamera.h"
#include "Runtime/Rendering/FRenderer.h"
#include "Runtime/Rendering/FRenderResourceLibrary.h"
#include "Runtime/Math/FMatrix.h"
#include "Runtime/Math/FVector4.h"
#include "Editor/Core/FEditor.h"
#include <numbers>

#include "Runtime/Engine/FRayCastingManager.h"


void FGrid::Initialize(FRenderResourceLibrary& RenderResources)
{
		GridMesh = RenderResources.GetGridMesh();
		GridMaterial = RenderResources.GetGridMaterial();

}

void FGrid::Draw(FRenderer& Renderer, const FCamera& Camera)
{
	if (!GridMesh || !GridMaterial) return;

	// 카메라 XY 따라감, Z=0 (바닥)
	const FMatrix World =
		FMatrix::MakeTranslation(FVector{ Camera.Position.X, Camera.Position.Y, 0.0f });

	FGridConstants C;
	C.World = World;
	C.MVP = World * Camera.CreateViewProjectionMatrix();  // 스왑은 UpdateGridConstants 가 함
	C.CellSize = 1.0f;

	Renderer.DrawGrid(*GridMesh, *GridMaterial, C);
}