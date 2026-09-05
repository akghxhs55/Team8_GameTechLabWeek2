#pragma once

#include "FVector.h"
#include <numbers>
#include <cmath>

// TODO: 스타일 정리 필요
struct FMatrix
{
	float M[4][4];

	static const FMatrix Identity;

	[[nodiscard]] FMatrix() = default;
	[[nodiscard]] FMatrix(const FVector& InX, const FVector& InY, const FVector& InZ, const FVector& InW);
	[[nodiscard]] explicit FMatrix(float N);

	static FMatrix GetIdentity()
	{
		FMatrix t;

		for (int i = 0; i < 4; ++i)
			for (int j = 0; j < 4; ++j)
				t.M[i][j] = (i == j) ? 1.0f : 0.0f;

		return t;
	}

	FMatrix Transpose() const
	{
		FMatrix result;

		for (int i = 0; i < 4; ++i)
			for (int j = 0; j < 4; ++j)
				result.M[i][j] = M[j][i];

		return result;
	}

	static FMatrix MakeScale(const FVector& S)
	{
		FMatrix R = GetIdentity();
		R.M[0][0] = S.X;
		R.M[1][1] = S.Y;
		R.M[2][2] = S.Z;
		return R;
	}

	static FMatrix MakeTranslation(const FVector& T)
	{
		FMatrix R = GetIdentity();
		R.M[3][0] = T.X;
		R.M[3][1] = T.Y;
		R.M[3][2] = T.Z;
		return R;
	}

	static FMatrix MakeRotationX(float Rad)
	{
		const float c = cosf(Rad), s = sinf(Rad);
		FMatrix R = GetIdentity();
		R.M[1][1] = c;  R.M[1][2] = s;
		R.M[2][1] = -s;  R.M[2][2] = c;
		return R;
	}

	static FMatrix MakeRotationY(float Rad)
	{
		const float c = cosf(Rad), s = sinf(Rad);
		FMatrix R = GetIdentity();
		R.M[0][0] = c;  R.M[0][2] = -s;
		R.M[2][0] = s;  R.M[2][2] = c;
		return R;
	}

	static FMatrix MakeRotationZ(float Rad)
	{
		const float c = cosf(Rad), s = sinf(Rad);
		FMatrix R = GetIdentity();
		R.M[0][0] = c;  R.M[0][1] = s;
		R.M[1][0] = -s;  R.M[1][1] = c;
		return R;
	}

	FMatrix operator*(const FMatrix& Other) const
	{
		FMatrix R;
		for (int i = 0; i < 4; ++i)
			for (int j = 0; j < 4; ++j)
				R.M[i][j] = M[i][0] * Other.M[0][j]
				+ M[i][1] * Other.M[1][j]
				+ M[i][2] * Other.M[2][j]
				+ M[i][3] * Other.M[3][j];
		return R;
	}

	static FMatrix MakeRotationXYZ(const FVector& Deg)
	{
		constexpr float DegToRad = std::numbers::pi_v<float> / 180.0f;
		return MakeRotationX(Deg.X * DegToRad)
			* MakeRotationY(Deg.Y * DegToRad)
			* MakeRotationZ(Deg.Z * DegToRad);
	}
};

inline const FMatrix FMatrix::Identity = FMatrix{
	FVector(1.0f, 0.0f, 0.0f),
	FVector(0.0f, 1.0f, 0.0f),
	FVector(0.0f, 0.0f, 1.0f),
	FVector(0.0f, 0.0f, 0.0f)
};

inline FMatrix::FMatrix(const FVector& InX, const FVector& InY, const FVector& InZ, const FVector& InW)
{
	M[0][0] = InX.X; M[0][1] = InX.Y; M[0][2] = InX.Z; M[0][3] = 0.0f;
	M[1][0] = InY.X; M[1][1] = InY.Y; M[1][2] = InY.Z; M[1][3] = 0.0f;
	M[2][0] = InZ.X; M[2][1] = InZ.Y; M[2][2] = InZ.Z; M[2][3] = 0.0f;
	M[3][0] = InW.X; M[3][1] = InW.Y; M[3][2] = InW.Z; M[3][3] = 1.0f;
}

inline FMatrix::FMatrix(float N)
{
	for (auto& i : M)
	{
		for (float& j : i)
		{
			j = N;
		}
	}
}
