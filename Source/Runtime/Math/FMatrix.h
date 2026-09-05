#pragma once

#include "TVector.h"
#include <concepts>

namespace Math
{
	template<typename T>
		requires std::is_floating_point_v<T>
	struct TMatrix
	{
		T M[4][4];

		static const TMatrix Identity;

		[[nodiscard]] TMatrix() = default;

		[[nodiscard]] TMatrix(const TVector<T>& InX, const TVector<T>& InY, const TVector<T>& InZ, const TVector<T>& InW);
	};

	template<typename T>
		requires std::is_floating_point_v<T>
	const TMatrix<T> TMatrix<T>::Identity = TMatrix<T>();

	template <typename T> requires std::is_floating_point_v<T>
	TMatrix<T>::TMatrix(const TVector<T>& InX, const TVector<T>& InY, const TVector<T>& InZ, const TVector<T>& InW)
	{
		M[0][0] = InX.X; M[0][1] = InX.Y; M[0][2] = InX.Z; M[0][3] = 0.0f;
		M[1][0] = InY.X; M[1][1] = InY.Y; M[1][2] = InY.Z; M[1][3] = 0.0f;
		M[2][0] = InZ.X; M[2][1] = InZ.Y; M[2][2] = InZ.Z; M[2][3] = 0.0f;
		M[3][0] = InW.X; M[3][1] = InW.Y; M[3][2] = InW.Z; M[3][3] = 1.0f;
	}
}
