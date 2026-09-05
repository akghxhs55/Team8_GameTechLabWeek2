#pragma once

#include "Runtime/Math/FVector.h"
#include "Runtime/Math/FQuaternion.h"
#include "Runtime/Math/FMatrix.h"

struct FTransform
{
	FVector Location{ 0.0, 0.0, 0.0 };
	FQuaternion Rotation{ 0.0, 0.0, 0.0, 1.0 };
	FVector Scale3D{ 1.0, 1.0, 1.0 };

	FMatrix ToMatrix() const;
};

// TODO: 구현
inline FMatrix FTransform::ToMatrix() const
{
	// 또는 Rotation.ToMatrix() ?
	return FMatrix::MakeScale(Scale3D) * Rotation.ToMatrixRow() * FMatrix::MakeTranslation(Location);
}
