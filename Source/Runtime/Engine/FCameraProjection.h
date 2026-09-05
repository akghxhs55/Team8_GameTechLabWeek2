#pragma once

#include "Runtime/Core/IntTypes.h"
#include "Runtime/Math/FMatrix.h"

enum class EProjectionType : uint8
{
	Perspective,
	Orthogonal,
};

struct FCameraProjection
{
	EProjectionType ProjectionType = EProjectionType::Perspective;
	// TODO: 뭐가 필요할까...
	float FOV;
	float NearPlane;
	float FarPlane;
	
	// TODO: 캐시 가능
	[[nodiscard]] FMatrix CreateProjectionMatrix() const;
};

inline FMatrix FCameraProjection::CreateProjectionMatrix() const
{
	// TODO: 구현
	// Sample camera: eye (-3, -3, 2), target (0, 0, 0), world up +Z.
	// Left-handed view space; row vectors match mul(Position, ViewProjection).
	FMatrix ProjectionMatrix(0.0f);
	ProjectionMatrix.M[0][0] = 1.29903811f;
	ProjectionMatrix.M[1][1] = 1.73205081f;
	ProjectionMatrix.M[2][2] = 1.00100100f;
	ProjectionMatrix.M[2][3] = 1.0f;
	ProjectionMatrix.M[3][2] = -0.10010010f;

	return ProjectionMatrix;
}
