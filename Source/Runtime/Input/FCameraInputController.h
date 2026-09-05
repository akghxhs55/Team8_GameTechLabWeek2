#pragma once

#include "Runtime/Math/FVector2.h"

struct FViewportCamera;

// TODO: 개선 요소 아주 많음
class FCameraInputController
{
public:
	void UpdateKeyInput(FViewportCamera& Camera, float DeltaTime) const;
	void HandleMouseInput(FViewportCamera& Camera, FVector2 MouseDelta) const;

	float CameraMoveSpeed = 10.0f;
	float CameraRotateSpeed = 0.2f;
};
