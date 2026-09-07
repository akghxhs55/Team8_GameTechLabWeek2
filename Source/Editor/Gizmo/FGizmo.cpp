#include "FGizmo.h"

#include "Runtime/CoreUObject/USceneComponent.h"
#include "Runtime/Engine/FCamera.h"
#include "Runtime/Rendering/FRenderer.h"

void FGizmo::SetTarget(const USceneComponent* Target)
{
	this->Target = Target;
}

void FGizmo::Draw(FRenderer& Renderer, const FCamera& Camera)
{
}
