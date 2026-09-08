#pragma once

#include "Runtime/CoreUObject/USceneComponent.h"
#include "Runtime/Core/IntTypes.h"
#include "Runtime/Core/PointerTypes.h"
#include "Runtime/Rendering/FMesh.h"
#include "Runtime/Rendering/FMaterial.h"

struct FRay;
struct FVector2;
struct FCamera;
class FRenderer;
class FRenderResourceLibrary;

enum class EGizmoHandle : uint8
{
	None = 0u,
	XAxis = 1u,
	YAxis = 2u,
	ZAxis = 3u,
};

// TODO: Rotation, Scale 추가
class FGizmo final
{
public:
	void Initialize(FRenderResourceLibrary& RenderResources); // TODO: 이거랑 메시 머티리얼 없애야 함...

	void SetTarget(USceneComponent* Target);
	void Draw(FRenderer& Renderer, const FCamera& Camera) const;

	EGizmoHandle HitTest(const FRay& Ray, const FCamera& Camera) const;
	void BeginInteraction(EGizmoHandle Handle);

	EGizmoHandle HoveredHandle = EGizmoHandle::None;
	EGizmoHandle ActiveHandle = EGizmoHandle::None;

private:
	// TODO: Axis enum으로 변경
	void DrawAxis(FRenderer& Renderer, EGizmoHandle Handle, const FMatrix& MVP) const;
	float CalculateGizmoScale(const FCamera& Camera) const;

private:
	USceneComponent* CurrentTarget = nullptr;

	TSharedPtr<FMesh> ArrowMesh;
	TSharedPtr<FMaterial> ArrowMaterial;
};
