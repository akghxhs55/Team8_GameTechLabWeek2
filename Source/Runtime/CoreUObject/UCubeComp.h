#pragma once

#include "UPrimitiveComponent.h"
#include "Runtime/Math/Vectors.h"

class UCubeComp : public UPrimitiveComponent
{
protected:
	explicit UCubeComp(FVector Location, float Size = 1.0f);

private:
	float Size = 1.0f;
};
