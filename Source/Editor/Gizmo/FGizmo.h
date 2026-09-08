#pragma once

#include "Runtime/CoreUObject/USceneComponent.h"
#include "Runtime/Core/IntTypes.h"
#include "Runtime/Core/PointerTypes.h"
#include "Runtime/Engine/FRayCastingManager.h"
#include "Runtime/Rendering/FMesh.h"
#include "Runtime/Rendering/FMaterial.h"

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

	[[nodiscard]] EGizmoHandle HitTest(const FRay& Ray, const FCamera& Camera) const;

	void BeginInteraction(EGizmoHandle Handle, const FVector2& MousePosition, const FCamera& Camera, const FVector2& ViewportSize);
	void UpdateInteraction(const FVector2& MousePosition);
	void EndInteraction();
	[[nodiscard]] bool IsInteracting() const { return ActiveHandle != EGizmoHandle::None; }

	EGizmoHandle HoveredHandle = EGizmoHandle::None;
	EGizmoHandle ActiveHandle = EGizmoHandle::None;

private:
	void DrawAxis(FRenderer& Renderer, EGizmoHandle Handle, const FMatrix& MVP) const;
	[[nodiscard]] float CalculateGizmoScale(const FCamera& Camera) const;
	[[nodiscard]] FVector2 WorldToViewport(const FVector& WorldPosition, const FCamera& Camera, const FVector2& ViewportSize) const;

private:
	USceneComponent* CurrentTarget = nullptr;

	TSharedPtr<FMesh> ArrowMesh;
	TSharedPtr<FMaterial> ArrowMaterial;

	FTransform InteractionStartTransform;
	FVector InteractionAxisWorld;
	FVector2 InteractionAxisScreen;
	FVector2 InteractionStartMouse;
	float InteractionWorldUnitsPerPixel = 0.0f;
};
