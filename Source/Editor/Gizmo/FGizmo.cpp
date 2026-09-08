#include "FGizmo.h"

#include "Runtime/Core/IntTypes.h"
#include "Runtime/CoreUObject/USceneComponent.h"
#include "Runtime/Engine/FCamera.h"
#include "Runtime/Rendering/FRenderer.h"
#include "Runtime/Rendering/FRenderResourceLibrary.h"
#include "Runtime/Math/FMatrix.h"
#include "Runtime/Math/FVector4.h"
#include <numbers>

#include "Runtime/Engine/FRayCastingManager.h"

void FGizmo::Initialize(FRenderResourceLibrary& RenderResources)
{
	ArrowMesh = RenderResources.GetArrowMesh();
	ArrowMaterial = RenderResources.GetSimpleMaterial();
}

void FGizmo::SetTarget(USceneComponent* Target)
{
	CurrentTarget = Target;
}

void FGizmo::Draw(FRenderer& Renderer, const FCamera& Camera) const
{
	if (!CurrentTarget)
	{
		return;
	}
	
	static FMatrix YAxisRotation = FMatrix::MakeRotationZ(std::numbers::pi_v<float> * 0.5f);
	static FMatrix ZAxisRotation = FMatrix::MakeRotationY(std::numbers::pi_v<float> * 0.5f);

	float GizmoScale = CalculateGizmoScale(Camera);
	FMatrix Translation = FMatrix::MakeTranslation(CurrentTarget->GetRelativeTransform().Location);
	FMatrix Scale = FMatrix::MakeScale(FVector{ GizmoScale, GizmoScale, GizmoScale });
	FMatrix VP = Camera.CreateViewProjectionMatrix();

	DrawAxis(Renderer, EGizmoHandle::XAxis, Scale * Translation * VP);
	DrawAxis(Renderer, EGizmoHandle::YAxis, Scale * YAxisRotation * Translation * VP);
	DrawAxis(Renderer, EGizmoHandle::ZAxis, Scale * ZAxisRotation * Translation * VP);
}

EGizmoHandle FGizmo::HitTest(const FRay& Ray, const FCamera& Camera) const
{
	if (!CurrentTarget)
	{
		return EGizmoHandle::None;
	}
	
	float GizmoScale = CalculateGizmoScale(Camera);

	static FMatrix YAxisRotation = FMatrix::MakeRotationZ(std::numbers::pi_v<float> *0.5f);
	static FMatrix ZAxisRotation = FMatrix::MakeRotationY(std::numbers::pi_v<float> *0.5f);

	FMatrix Translation = FMatrix::MakeTranslation(CurrentTarget->GetRelativeTransform().Location);
	FMatrix Scale = FMatrix::MakeScale(FVector{ GizmoScale, GizmoScale, GizmoScale });

	float ClosestDistance = (std::numeric_limits<float>::max)();
	EGizmoHandle ClosestHandle = EGizmoHandle::None;

	float HitDistance;
	FVector ImpactPoint;
	if (FRayCastingManager::RayIntersectsMesh(
			Ray,
			*ArrowMesh,
			Scale * Translation,
			HitDistance,
			ImpactPoint) &&
		HitDistance < ClosestDistance)
	{
		ClosestDistance = HitDistance;
		ClosestHandle = EGizmoHandle::XAxis;
	}
	if (FRayCastingManager::RayIntersectsMesh(
		Ray,
		*ArrowMesh,
		Scale * YAxisRotation * Translation,
		HitDistance,
		ImpactPoint) &&
		HitDistance < ClosestDistance)
	{
		ClosestDistance = HitDistance;
		ClosestHandle = EGizmoHandle::YAxis;
	}
	if (FRayCastingManager::RayIntersectsMesh(
		Ray,
		*ArrowMesh,
		Scale * ZAxisRotation * Translation,
		HitDistance,
		ImpactPoint) &&
		HitDistance < ClosestDistance)
	{
		ClosestDistance = HitDistance;
		ClosestHandle = EGizmoHandle::ZAxis;
	}

	return ClosestHandle;
}

void FGizmo::BeginInteraction(EGizmoHandle Handle, const FVector2& MousePosition, const FCamera& Camera, const FVector2& ViewportSize)
{
	if (!CurrentTarget)
	{
		return;
	}

	FVector AxisWorld;
	switch (Handle)
	{
	case EGizmoHandle::XAxis:
		AxisWorld = FVector{ 1.0f, 0.0f, 0.0f };
		break;
	case EGizmoHandle::YAxis:
		AxisWorld = FVector{ 0.0f, 1.0f, 0.0f };
		break;
	case EGizmoHandle::ZAxis:
		AxisWorld = FVector{ 0.0f, 0.0f, 1.0f };
		break;
	case EGizmoHandle::None:
		return;
	}

	InteractionStartTransform = CurrentTarget->GetRelativeTransform();
	InteractionStartMouse = MousePosition;
	InteractionAxisWorld = AxisWorld;

	float GizmoScale = CalculateGizmoScale(Camera);

	FVector OriginWorld = CurrentTarget->GetRelativeTransform().Location;
	FVector AxisEndWorld = OriginWorld + AxisWorld * GizmoScale;

	FVector2 OriginScreen = WorldToViewport(OriginWorld, Camera, ViewportSize);
	FVector2 AxisEndScreen = WorldToViewport(AxisEndWorld, Camera, ViewportSize);

	FVector2 AxisScreen = AxisEndScreen - OriginScreen;
	float AxisScreenLength = AxisScreen.Size();

	if (AxisScreenLength > 1e-5f)
	{
		InteractionAxisScreen = AxisScreen / AxisScreenLength;
		InteractionWorldUnitsPerPixel = GizmoScale / AxisScreenLength;
	}

	ActiveHandle = Handle;
}

void FGizmo::UpdateInteraction(const FVector2& MousePosition)
{
	if (!CurrentTarget || ActiveHandle == EGizmoHandle::None)
	{
		return;
	}

	FVector2 MouseDelta = MousePosition - InteractionStartMouse;
	float ScreenDistance = MouseDelta.Dot(InteractionAxisScreen);
	float WorldDistance = ScreenDistance * InteractionWorldUnitsPerPixel;

	FTransform Transform = InteractionStartTransform;
	Transform.Location += InteractionAxisWorld * WorldDistance;
	CurrentTarget->SetRelativeTransform(Transform);
}

void FGizmo::EndInteraction()
{
	ActiveHandle = EGizmoHandle::None;
}

void FGizmo::DrawAxis(FRenderer& Renderer, EGizmoHandle Handle, const FMatrix& MVP) const
{
	constexpr FVector Color[3] = {
		FVector{ 0.8f, 0.0f, 0.0f },
		FVector{ 0.0f, 0.8f, 0.0f },
		FVector{ 0.0f, 0.0f, 0.8f },
	};

	constexpr FVector ActiveColor = FVector{ 1.0f, 1.0f, 0.1f };
	constexpr FVector HoverColor = FVector{ 0.7f, 0.7f, 0.0f };

	if (ActiveHandle == Handle)
	{
		Renderer.Draw(*ArrowMesh, *ArrowMaterial, { MVP, ActiveColor, 1.0f });
	}
	else if (HoveredHandle == Handle)
	{
		Renderer.Draw(*ArrowMesh, *ArrowMaterial, { MVP, HoverColor, 1.0f });
	}
	else
	{
		Renderer.Draw(*ArrowMesh, *ArrowMaterial, { MVP, Color[static_cast<uint8>(Handle) - 1], 1.0f });
	}
}

float FGizmo::CalculateGizmoScale(const FCamera& Camera) const
{
	constexpr float ScalePerDistance = 0.15f;

	FVector ToTarget = CurrentTarget->GetRelativeTransform().Location - Camera.Position;

	return ToTarget.Size() * ScalePerDistance;
}

FVector2 FGizmo::WorldToViewport(const FVector& WorldPosition, const FCamera& Camera,
	const FVector2& ViewportSize) const
{
	FMatrix VP = Camera.CreateViewProjectionMatrix();

	FVector Projected = VP.TransformPointRow(WorldPosition);

	return FVector2{
		(Projected.Y + 1.0f) * 0.5f * ViewportSize.X,
		(1.0f - Projected.Z) * 0.5f * ViewportSize.Y
	};
}
