#pragma once

#include "Runtime/CoreUObject/UPrimitiveComponent.h"
#include "Runtime/Math/FVector2.h"

struct FCamera;
class FRenderer;
class FGizmo;

class FRenderView final {
	FRenderer& Renderer;

public:
	FRenderView(FRenderer& Renderer);
	void Render(const FCamera& Camera, FVector2 TopLeft, FVector2 Length, UPrimitiveComponent* Rendered);
	void RenderGizmo(const FCamera& Camera, FVector2 TopLeft, FVector2 Size, const FGizmo& Gizmo);
};