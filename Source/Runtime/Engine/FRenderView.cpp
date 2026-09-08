#include "FRenderView.h"

#include "Runtime/Rendering/FRenderer.h"
#include "Runtime/Engine/FCamera.h"
#include "Runtime/Math/FVector2.h"
#include "Editor/Gizmo/FGizmo.h"

FRenderView::FRenderView(FRenderer& Renderer):
	Renderer(Renderer)
{
}

void FRenderView::Render(const FCamera& Camera, FVector2 TopLeft, FVector2 Length, UPrimitiveComponent* Rendered)
{
	// TODO: 렌더뷰가 렌더러 구현을 알게 해서 여기서 V, P 따로 받고 월드축변환행렬을 곱하거나,
	// 렌더러쪽 UpdateObjectConstants를 Draw함수 안에 숨긴뒤 인수로 M, V, P와 월드축을 받게 하면 렌더뷰도 렌더러 구현 모름
	const FMatrix VP = Camera.CreateViewProjectionMatrix();
	Renderer.Draw(*Rendered->GetMesh(), *Rendered->GetMaterial(), { Rendered->RelativeTransform.ToMatrix() * VP });
}

void FRenderView::RenderGizmo(const FTransform& Transform, const FCamera& Camera, FVector2 TopLeft, FVector2 Size, const FGizmo& Gizmo)
{
	Gizmo.Draw(Transform, Renderer, Camera);
}
