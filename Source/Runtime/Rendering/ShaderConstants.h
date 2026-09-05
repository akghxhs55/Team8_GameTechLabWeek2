#pragma once

#include "Runtime/Math/MatrixTypes.h"

// b0에 바인딩
struct FFrameConstants
{
	FMatrix ViewProjection;
};
static_assert(sizeof(FFrameConstants) % 16 == 0);

// b1에 바인딩
struct FObjectConstants
{
	FMatrix Model;
};
static_assert(sizeof(FObjectConstants) % 16 == 0);
