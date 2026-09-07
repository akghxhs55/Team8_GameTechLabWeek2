#pragma once

#include "Runtime/Math/FVector.h"
#include "Runtime/Math/FMatrix.h"

// b0에 바인딩
struct FFrameConstants
{
	// FMatrix ViewProjection;
};
// static_assert(sizeof(FFrameConstants) % 16 == 0);

// b1에 바인딩
struct FObjectConstants
{
	FMatrix MVP;
	// FMatrix Model;
	FVector Color;

	float Padding;
};
static_assert(sizeof(FObjectConstants) % 16 == 0);
