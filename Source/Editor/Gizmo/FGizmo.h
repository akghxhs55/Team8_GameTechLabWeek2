#pragma once

#include "Runtime/CoreUObject/USceneComponent.h"

struct FCamera;
class FRenderer;

// TODO: Rotation, Scale 추가
class FGizmo final
{
public:
	void SetTarget(const USceneComponent* Target);
	void Draw(FRenderer& Renderer, const FCamera& Camera);

private:
	const USceneComponent* Target = nullptr;
};
