#pragma once

#include "Runtime/Math/FVector.h"
#include "Runtime/Math/FMatrix.h"

// b0에 바인딩
struct FObjectConstants
{
	FMatrix MVP;
	FVector ColorOverride{ 0.0f, 0.0f, 0.0f };
	float ColorOverrideAmount = 0.0f;
};
static_assert(sizeof(FObjectConstants) % 16 == 0);

//b1에 바인딩
struct FGridConstants
{
    FMatrix MVP;
	FMatrix World;
    float CellSize;
	FVector pad;
};
