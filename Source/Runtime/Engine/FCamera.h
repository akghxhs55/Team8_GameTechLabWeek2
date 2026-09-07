#pragma once

#include "FCameraProjection.h"
#include "Runtime/Math/FVector.h"
#include "Runtime/Math/FMatrix.h"

struct FCamera
{
	FVector Position{ 0.0f, 0.0f, 0.0f };
	float Yaw = 0.0f;
	float Pitch = 0.0f;
	FCameraProjection Projection;

	// TODO: 캐시 가능, 캐시하려면 세터를 넣어야 함
	[[nodiscard]] FMatrix CreateViewProjectionMatrix() const;

	// Move(), Rotate(), Zoom() 등 추가 가능
};

// TODO: 카메라가 렌더링엔진 좌표계를 모르게 하기(렌더러가 엔진의 좌표계 정보를 받아 변환하는 것이 바람직)
// TODO: 실제 화면비에 따른 처리(FCameraProjection이 ratio를 들고 있는것이 의미없는것은 아닐 수 있으나, 실화면비와의 관계 정립이 필요)
inline FMatrix FCamera::CreateViewProjectionMatrix() const
{
	// 언리얼 카메라 좌표계 -> HLSL 카메라 좌표계
	static const FMatrix CameraToProjectionAxes{
		FVector{ 0.0f, 0.0f, 1.0f },
		FVector{ 1.0f, 0.0f, 0.0f },
		FVector{ 0.0f, 1.0f, 0.0f },
		FVector{ 0.0f, 0.0f, 0.0f }
	};

	const FMatrix InverseRotationMatrix = FMatrix::MakeRotation(FVector(0.0f, Pitch, Yaw)).Transpose();

	// (R * T)^-1 = T^-1 * R^-1 = T^-1 * R^T
	const FMatrix ViewMatrix = FMatrix::MakeTranslation(-Position) * InverseRotationMatrix;

	const FMatrix ProjectionMatrix = Projection.CreateProjectionMatrix();

	return ViewMatrix * CameraToProjectionAxes * ProjectionMatrix;
}
