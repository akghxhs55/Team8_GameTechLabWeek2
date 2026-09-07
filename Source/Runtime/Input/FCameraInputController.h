#pragma once

#include "Runtime/Math/FVector2.h"

struct FCamera;

// TODO: 개선 요소 아주 많음
class FCameraInputController
{
public:
	void UpdateKeyInput(FCamera& Camera, float DeltaTime) const;
	void HandleMouseInput(FCamera& Camera, float DeltaTime, FVector2 MouseDelta) const;

	float CameraMoveSpeed = 10.0f;
	float CameraRotateSpeed = 100.0f;
};
