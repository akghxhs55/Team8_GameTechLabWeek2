#pragma once
#include "Runtime/Math/FVector2.h"
#include "Runtime/Engine/FCamera.h"

class FEditorViewport final {
	bool bFocused = false;
	bool bHovered = false;
public:
	FCamera ViewportCamera;
	FVector2 TopLeft = { 0.0f, 0.0f };
	FVector2 Length = { 1.0f, 1.0f };
	void UpdateFocusedAndHovered(bool bFocused, bool bHovered);
};