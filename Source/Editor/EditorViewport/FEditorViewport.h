#pragma once
#include "Runtime/Math/FVector2.h"
#include "Runtime/Engine/FCamera.h"

class FEditorViewport final {
	bool bFocused = false;
	bool bHovered = false;
public:
	FCamera ViewportCamera;
	FVector2 TopLeftUV = { 0.0f, 0.0f };
	FVector2 LengthUV = { 1.0f, 1.0f };
	void UpdateFocusedAndHovered(bool bFocused, bool bHovered);
	

	[[nodiscard]] bool IsFocused() const { return bFocused; }
	[[nodiscard]] bool IsHovered() const { return bHovered; }
};