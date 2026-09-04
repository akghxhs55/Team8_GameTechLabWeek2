#pragma once

#include "UObject.h"
#include "Runtime/Math/Vectors.h"

class USceneComponent : public UObject
{
public:
	FVector RelativeLocation{ 0.0, 0.0, 0.0 };
	FVector RelativeRotation{ 0.0, 0.0, 0.0 };
	FVector RelativeScale3D{ 1.0, 1.0, 1.0 };

protected:
	USceneComponent() = default;
};
