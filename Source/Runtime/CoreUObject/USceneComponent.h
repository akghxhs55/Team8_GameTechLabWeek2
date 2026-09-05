#pragma once

#include "UObject.h"
#include "Runtime/Geometry/FTransform.h"

class USceneComponent : public UObject
{
public:
	FTransform RelativeTransform;

protected:
	USceneComponent() = default;
};
