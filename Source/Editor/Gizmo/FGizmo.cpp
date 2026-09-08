#include "FGizmo.h"

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

void FGizmo::Initialize(FRenderResourceLibrary& RenderResources)
{
	ArrowMesh = RenderResources.GetArrowMesh();
	CircleMesh = RenderResources.GetCircleMesh();
	SquareArrowMesh = RenderResources.GetSquareArrowMesh();
	GizmoMaterial = RenderResources.GetDrawOverMaterial();
}

void FGizmo::Draw(const FTransform& Transform, FRenderer& Renderer, const FCamera& Camera) const
{
	static FMatrix YAxisRotation = FMatrix::MakeRotationZ(std::numbers::pi_v<float> * 0.5f);
	static FMatrix ZAxisRotation = FMatrix::MakeRotationY(std::numbers::pi_v<float> * 0.5f);

	float GizmoScale = CalculateGizmoScale(Transform.Location, Camera);
	FMatrix Scale = FMatrix::MakeScale(FVector{ GizmoScale, GizmoScale, GizmoScale });
	FMatrix ObjectRotation = GetSpace() == EGizmoSpace::World ? FMatrix::GetIdentity() : Transform.Rotation.ToMatrixRow();
	FMatrix Translation = FMatrix::MakeTranslation(Transform.Location);
	FMatrix VP = Camera.CreateViewProjectionMatrix();

	DrawAxis(Renderer, EGizmoHandle::XAxis, Scale * ObjectRotation * Translation * VP);
	DrawAxis(Renderer, EGizmoHandle::YAxis, Scale * YAxisRotation * ObjectRotation * Translation * VP);
	DrawAxis(Renderer, EGizmoHandle::ZAxis, Scale * ZAxisRotation * ObjectRotation * Translation * VP);
}

EGizmoHandle FGizmo::HitTest(FEditor& Editor, const FRay& Ray, const FCamera& Camera) const
{
	if (!Editor.ObjectSelected())
	{
		return EGizmoHandle::None;
	}
	
	float GizmoScale = CalculateGizmoScale(Editor.SelectedTransform.Location, Camera);

	static FMatrix YAxisRotation = FMatrix::MakeRotationZ(std::numbers::pi_v<float> * 0.5f);
	static FMatrix ZAxisRotation = FMatrix::MakeRotationY(std::numbers::pi_v<float> * 0.5f);

	FMatrix Scale = FMatrix::MakeScale(FVector{ GizmoScale, GizmoScale, GizmoScale });
	FMatrix ObjectRotation = GetSpace() == EGizmoSpace::World ? FMatrix::GetIdentity() : Editor.SelectedTransform.Rotation.ToMatrixRow();
	FMatrix Translation = FMatrix::MakeTranslation(Editor.SelectedTransform.Location);

	float ClosestDistance = (std::numeric_limits<float>::max)();
	EGizmoHandle ClosestHandle = EGizmoHandle::None;

	TSharedPtr<FMesh> GizmoMesh;
	switch (Mode)
	{
	case EGizmoMode::Translate:
		GizmoMesh = ArrowMesh;
		break;

	case EGizmoMode::Rotate:
		GizmoMesh = CircleMesh;
		break;

	case EGizmoMode::Scale:
		GizmoMesh = SquareArrowMesh;
		break;

	case EGizmoMode::None:
		return EGizmoHandle::None;
	}

	float HitDistance;
	FVector ImpactPoint;
	if (FRayCastingManager::RayIntersectsMesh(
			Ray,
			*GizmoMesh,
			Scale * ObjectRotation * Translation,
			HitDistance,
			ImpactPoint) &&
		HitDistance < ClosestDistance)
	{
		ClosestDistance = HitDistance;
		ClosestHandle = EGizmoHandle::XAxis;
	}
	if (FRayCastingManager::RayIntersectsMesh(
		Ray,
		*GizmoMesh,
		Scale * YAxisRotation * ObjectRotation * Translation,
		HitDistance,
		ImpactPoint) &&
		HitDistance < ClosestDistance)
	{
		ClosestDistance = HitDistance;
		ClosestHandle = EGizmoHandle::YAxis;
	}
	if (FRayCastingManager::RayIntersectsMesh(
		Ray,
		*GizmoMesh,
		Scale * ZAxisRotation * ObjectRotation * Translation,
		HitDistance,
		ImpactPoint) &&
		HitDistance < ClosestDistance)
	{
		ClosestDistance = HitDistance;
		ClosestHandle = EGizmoHandle::ZAxis;
	}

	return ClosestHandle;
}

void FGizmo::BeginInteraction(FEditor& Editor, EGizmoHandle Handle, const FVector2& MousePosition, const FCamera& Camera, const FVector2& ViewportSize)
{
	if (!Editor.ObjectSelected())
	{
		return;
	}

	switch (Handle)
	{
	case EGizmoHandle::XAxis:
		InteractionAxisLocal = FVector{ 1.0f, 0.0f, 0.0f };
		break;
	case EGizmoHandle::YAxis:
		InteractionAxisLocal = FVector{ 0.0f, 1.0f, 0.0f };
		break;
	case EGizmoHandle::ZAxis:
		InteractionAxisLocal = FVector{ 0.0f, 0.0f, 1.0f };
		break;
	case EGizmoHandle::None:
		return;
	}
	InteractionAxisWorld = GetSpace() == EGizmoSpace::World ? InteractionAxisLocal : Editor.SelectedTransform.Rotation.RotateVector(InteractionAxisLocal);

	InteractionStartTransform = Editor.SelectedTransform;
	InteractionStartMouse = MousePosition;

	float GizmoScale = CalculateGizmoScale(Editor.SelectedTransform.Location, Camera);

	FVector OriginWorld = Editor.SelectedTransform.Location;
	FVector AxisEndWorld = OriginWorld + InteractionAxisWorld * GizmoScale;

	FVector2 OriginScreen = WorldToViewport(OriginWorld, Camera, ViewportSize);
	FVector2 AxisEndScreen = WorldToViewport(AxisEndWorld, Camera, ViewportSize);

	FVector2 AxisScreen = AxisEndScreen - OriginScreen;
	float AxisScreenLength = AxisScreen.Size();

	InteractionOriginViewport = OriginScreen;

	FVector CenterToCamera = Camera.Position - OriginWorld;
	InteractionRotationSign = (CenterToCamera.Dot(InteractionAxisWorld) <= 0.0f) ? 1.0f : -1.0f;

	if (AxisScreenLength > 1e-5f)
	{
		InteractionAxisViewport = AxisScreen / AxisScreenLength;
		InteractionWorldUnitsPerPixel = GizmoScale / AxisScreenLength;
		ActiveHandle = Handle;
	}
}

void FGizmo::UpdateInteraction(FEditor& Editor, const FVector2& MousePosition)
{
	if (!Editor.ObjectSelected() || ActiveHandle == EGizmoHandle::None)
	{
		return;
	}

	FVector2 MouseDelta = MousePosition - InteractionStartMouse;
	float ScreenDistance = MouseDelta.Dot(InteractionAxisViewport);
	float WorldDistance = ScreenDistance * InteractionWorldUnitsPerPixel;

	switch (Mode)
	{
	case EGizmoMode::Translate:
		Editor.SelectedTransform.Location = InteractionStartTransform.Location + InteractionAxisWorld * WorldDistance;
		break;
		
	case EGizmoMode::Rotate:
	{
		FVector2 BA = InteractionStartMouse - InteractionOriginViewport;
		FVector2 BC = MousePosition - InteractionOriginViewport;
		float Theta = (std::atan2f(BA.Y, BA.X) - std::atan2f(BC.Y, BC.X)) * InteractionRotationSign * 180.0f / std::numbers::pi_v<float>;
		if (GetSpace() == EGizmoSpace::World)
		{
			FQuaternion Delta = FQuaternion::FromAxisAngle(InteractionAxisWorld, Theta);
			Editor.SelectedTransform.Rotation = Delta * InteractionStartTransform.Rotation;
		}
		else
		{
			FQuaternion Delta = FQuaternion::FromAxisAngle(InteractionAxisLocal, Theta);
			Editor.SelectedTransform.Rotation = InteractionStartTransform.Rotation * Delta;
		}
		Editor.SelectedEulerDegDisplay = Editor.SelectedTransform.Rotation.GetEulerXYZ() * 180.0f / std::numbers::pi_v<float>;
		break;
	}

	case EGizmoMode::Scale:
		Editor.SelectedTransform.Scale3D = InteractionStartTransform.Scale3D + InteractionAxisLocal * WorldDistance;
		break;

	case EGizmoMode::None:
		return;
	}
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

	TSharedPtr<FMesh> GizmoMesh;
	switch (Mode)
	{
	case EGizmoMode::Translate:
		GizmoMesh = ArrowMesh;
		break;
	case EGizmoMode::Rotate:
		GizmoMesh = CircleMesh;
		break;
	case EGizmoMode::Scale:
		GizmoMesh = SquareArrowMesh;
		break;
	case EGizmoMode::None:
		return;
	}

	if (ActiveHandle == Handle)
	{
		Renderer.Draw(*GizmoMesh, *GizmoMaterial, { MVP, ActiveColor, 1.0f });
	}
	else if (HoveredHandle == Handle)
	{
		Renderer.Draw(*GizmoMesh, *GizmoMaterial, { MVP, HoverColor, 1.0f });
	}
	else
	{
		Renderer.Draw(*GizmoMesh, *GizmoMaterial, { MVP, Color[static_cast<uint8>(Handle) - 1], 1.0f });
	}
}

float FGizmo::CalculateGizmoScale(const FVector& GizmoLocation, const FCamera& Camera) const
{
	constexpr float ScalePerDistance = 0.15f;

	FVector ToTarget = GizmoLocation - Camera.Position;

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
