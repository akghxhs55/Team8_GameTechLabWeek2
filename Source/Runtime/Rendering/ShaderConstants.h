#pragma once

#include "Runtime/Math/FVector.h"
#include "Runtime/Math/FMatrix.h"

// b0에 바인딩
struct FObjectConstants
{
	FMatrix MVP;
	FVector Color{ 0.0f, 0.0f, 0.0f };

	float Padding = 0.0f;
};
static_assert(sizeof(FObjectConstants) % 16 == 0);
