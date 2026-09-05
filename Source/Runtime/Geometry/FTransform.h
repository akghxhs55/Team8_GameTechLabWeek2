#pragma once

#include "Runtime/Math/VectorTypes.h"
#include "Runtime/Math/MatrixTypes.h"

struct FTransform
{
	FVector Location{ 0.0, 0.0, 0.0 };
	FVector Rotation{ 0.0, 0.0, 0.0 };
	FVector Scale3D{ 1.0, 1.0, 1.0 };

	FMatrix ToMatrix() const;
};

inline FMatrix FTransform::ToMatrix() const
{
	return FMatrix{};
}
