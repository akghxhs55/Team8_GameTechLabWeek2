#pragma once
#include "Runtime/Rendering/FRenderer.h"
#include "FCamera.h"
#include "Runtime/CoreUObject/UPrimitiveComponent.h"
#include "Runtime/Math/FVector2.h"

class FRenderView final {
	FRenderer& Renderer;
public:
	FRenderView(FRenderer& Renderer);
	void Render(const FCamera& Camera, FVector2 TopLeft, FVector2 Length, UPrimitiveComponent* Rendered);
};