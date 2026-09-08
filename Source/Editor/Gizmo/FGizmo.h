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
class FEditor;

enum class EGizmoMode : uint8
{
	None = 0u,
	Translate = 1u,
	Rotate = 2u,
	Scale = 3u,
};

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

	void Draw(const FVector& Location, FRenderer& Renderer, const FCamera& Camera) const;

	[[nodiscard]] EGizmoHandle HitTest(FEditor& Editor, const FRay& Ray, const FCamera& Camera) const;

	void BeginInteraction(FEditor& Editor, EGizmoHandle Handle, const FVector2& MousePosition, const FCamera& Camera, const FVector2& ViewportSize);
	void UpdateInteraction(FEditor& Editor, const FVector2& MousePosition);
	void EndInteraction();
	[[nodiscard]] bool IsInteracting() const { return ActiveHandle != EGizmoHandle::None; }

	EGizmoMode Mode = EGizmoMode::Rotate;

	EGizmoHandle HoveredHandle = EGizmoHandle::None;
	EGizmoHandle ActiveHandle = EGizmoHandle::None;

private:
	void DrawAxis(FRenderer& Renderer, EGizmoHandle Handle, const FMatrix& MVP) const;
	[[nodiscard]] float CalculateGizmoScale(const FVector& GizmoLocation, const FCamera& Camera) const;
	[[nodiscard]] FVector2 WorldToViewport(const FVector& WorldPosition, const FCamera& Camera, const FVector2& ViewportSize) const;

private:
	TSharedPtr<FMesh> ArrowMesh;
	TSharedPtr<FMesh> CircleMesh;
	TSharedPtr<FMesh> SquareArrowMesh;
	TSharedPtr<FMaterial> GizmoMaterial;

	FTransform InteractionStartTransform;
	FVector InteractionAxisWorld;
	FVector2 InteractionAxisViewport;
	FVector2 InteractionStartMouse;
	FVector2 InteractionOriginViewport;
	float InteractionRotationSign = 1.0f;
	float InteractionWorldUnitsPerPixel = 0.0f;
};
