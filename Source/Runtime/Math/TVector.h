#pragma once

#include <cassert>
#include <cmath>
#include <cstdint>
#include <type_traits>

namespace Math
{
	template<typename T> 
		requires std::is_floating_point_v<T>
	struct TVector
	{
		T X;
		T Y;
		T Z;

		static const TVector ZeroVector;
		static const TVector OneVector;
		static const TVector UpVector;
		static const TVector DownVector;
		static const TVector ForwardVector;
		static const TVector BackwardVector;
		static const TVector RightVector;
		static const TVector LeftVector;

		[[nodiscard]] constexpr TVector(T InX, T InY, T InZ);
		// [[nodiscard]] explicit TVector(const TVector2<T>& V, T InZ);
		// [[nodiscard]] TVector(const TVector4<T>& V);

		[[nodiscard]] TVector operator+(const TVector& V) const;
		
		template<typename ScalarType> requires std::is_arithmetic_v<ScalarType>
		[[nodiscard]] TVector operator+(ScalarType Scale) const;

		[[nodiscard]] TVector operator-(const TVector& V) const;

		template<typename ScalarType> requires std::is_arithmetic_v<ScalarType>
		[[nodiscard]] TVector operator-(ScalarType Scale) const;

		[[nodiscard]] TVector operator*(const TVector& V) const;

		template<typename ScalarType> requires std::is_arithmetic_v<ScalarType>
		[[nodiscard]] TVector operator*(ScalarType Scale) const;

		template<typename ScalarType> requires std::is_arithmetic_v<ScalarType>
		[[nodiscard]] TVector operator/(ScalarType Scale) const;

		[[nodiscard]] bool operator==(const TVector& V) const;

		[[nodiscard]] bool operator!=(const TVector& V) const;

		[[nodiscard]] TVector operator-() const;

		TVector& operator+=(const TVector& V);

		template<typename ScalarType> requires std::is_arithmetic_v<ScalarType>
			TVector& operator+=(ScalarType Scale);

		TVector& operator-=(const TVector& V);

		template<typename ScalarType> requires std::is_arithmetic_v<ScalarType>
			TVector& operator-=(ScalarType Scale);

		TVector& operator*=(const TVector& V);

		template<typename ScalarType> requires std::is_arithmetic_v<ScalarType>
		TVector& operator*=(ScalarType Scale);

		TVector& operator/=(const TVector& V);

		template<typename ScalarType> requires std::is_arithmetic_v<ScalarType>
		TVector& operator/=(ScalarType Scale);

		[[nodiscard]] T& operator[](int32_t Index);
		[[nodiscard]] const T& operator[](int32_t Index) const;

		[[nodiscard]] TVector Cross(const TVector& V) const;

		[[nodiscard]] T Dot(const TVector& V) const;

		[[nodiscard]] T Size() const;
		[[nodiscard]] T SizeSquared() const;
	};

	template <typename T> requires std::is_floating_point_v<T>
	const TVector<T> TVector<T>::ZeroVector(0, 0, 0);

	template <typename T> requires std::is_floating_point_v<T>
	const TVector<T> TVector<T>::OneVector(1, 1, 1);

	template <typename T> requires std::is_floating_point_v<T>
	const TVector<T> TVector<T>::UpVector(0, 0, 1);

	template <typename T> requires std::is_floating_point_v<T>
	const TVector<T> TVector<T>::DownVector(0, 0, -1);

	template <typename T> requires std::is_floating_point_v<T>
	const TVector<T> TVector<T>::ForwardVector(1, 0, 0);

	template <typename T> requires std::is_floating_point_v<T>
	const TVector<T> TVector<T>::BackwardVector(-1, 0, 0);

	template <typename T> requires std::is_floating_point_v<T>
	const TVector<T> TVector<T>::RightVector(0, 1, 0);

	template <typename T> requires std::is_floating_point_v<T>
	const TVector<T> TVector<T>::LeftVector(0, -1, 0);

	template <typename T> requires std::is_floating_point_v<T>
	constexpr TVector<T>::TVector(T InX, T InY, T InZ)
		: X(InX), Y(InY), Z(InZ)
	{}

	template <typename T> requires std::is_floating_point_v<T>
	TVector<T> TVector<T>::operator+(const TVector& V) const
	{
		return TVector(X + V.X, Y + V.Y, Z + V.Z);
	}

	template <typename T> requires std::is_floating_point_v<T>
	template <typename ScalarType> 
	requires std::is_arithmetic_v<ScalarType>
	TVector<T> TVector<T>::operator+(ScalarType Scale) const
	{
		return TVector(X + Scale, Y + Scale, Z + Scale);
	}

	template <typename T> requires std::is_floating_point_v<T>
	TVector<T> TVector<T>::operator-(const TVector& V) const
	{
		return TVector(X - V.X, Y - V.Y, Z - V.Z);
	}

	template <typename T> requires std::is_floating_point_v<T>
	template <typename ScalarType> requires std::is_arithmetic_v<ScalarType>
	TVector<T> TVector<T>::operator-(ScalarType Scale) const
	{
		return TVector(X - Scale, Y - Scale, Z - Scale);
	}

	template <typename T> requires std::is_floating_point_v<T>
	TVector<T> TVector<T>::operator*(const TVector& V) const
	{
		return TVector(X * V.X, Y * V.Y, Z * V.Z);
	}

	template <typename T> requires std::is_floating_point_v<T>
	template <typename ScalarType> requires std::is_arithmetic_v<ScalarType>
	TVector<T> TVector<T>::operator*(ScalarType Scale) const
	{
		const T TypedScale = static_cast<T>(Scale);
		return TVector(X * TypedScale, Y * TypedScale, Z * TypedScale);
	}

	template <typename T> requires std::is_floating_point_v<T>
	template <typename ScalarType> requires std::is_arithmetic_v<ScalarType>
	TVector<T> TVector<T>::operator/(ScalarType Scale) const
	{
		const T TypedScale = static_cast<T>(Scale);
		return TVector(X / TypedScale, Y / TypedScale, Z / TypedScale);
	}

	template <typename T> requires std::is_floating_point_v<T>
	bool TVector<T>::operator==(const TVector& V) const
	{
		return X == V.X && Y == V.Y && Z == V.Z;
	}

	template <typename T> requires std::is_floating_point_v<T>
	bool TVector<T>::operator!=(const TVector& V) const
	{
		return !(*this == V);
	}

	template <typename T> requires std::is_floating_point_v<T>
	TVector<T> TVector<T>::operator-() const
	{
		return TVector(-X, -Y, -Z);
	}

	template <typename T> requires std::is_floating_point_v<T>
	TVector<T>& TVector<T>::operator+=(const TVector& V)
	{
		X += V.X; Y += V.Y; Z += V.Z;
		return *this;
	}

	template <typename T> requires std::is_floating_point_v<T>
	template <typename ScalarType> requires std::is_arithmetic_v<ScalarType>
	TVector<T>& TVector<T>::operator+=(ScalarType Scale)
	{
		const T TypedScale = static_cast<T>(Scale);
		X += TypedScale; Y += TypedScale; Z += TypedScale;
		return *this;
	}

	template <typename T> requires std::is_floating_point_v<T>
	TVector<T>& TVector<T>::operator-=(const TVector& V)
	{
		X -= V.X; Y -= V.Y; Z -= V.Z;
		return *this;
	}

	template <typename T> requires std::is_floating_point_v<T>
	template <typename ScalarType> requires std::is_arithmetic_v<ScalarType>
	TVector<T>& TVector<T>::operator-=(ScalarType Scale)
	{
		const T TypedScale = static_cast<T>(Scale);
		X -= TypedScale; Y -= TypedScale; Z -= TypedScale;
		return *this;
	}

	template <typename T> requires std::is_floating_point_v<T>
	TVector<T>& TVector<T>::operator*=(const TVector& V)
	{
		X *= V.X; Y *= V.Y; Z *= V.Z;
		return *this;
	}

	template <typename T> requires std::is_floating_point_v<T>
	template <typename ScalarType> requires std::is_arithmetic_v<ScalarType>
	TVector<T>& TVector<T>::operator*=(ScalarType Scale)
	{
		const T TypedScale = static_cast<T>(Scale);
		X *= TypedScale; Y *= TypedScale; Z *= TypedScale;
		return *this;
	}

	template <typename T> requires std::is_floating_point_v<T>
	TVector<T>& TVector<T>::operator/=(const TVector& V)
	{
		X /= V.X; Y /= V.Y; Z /= V.Z;
		return *this;
	}

	template <typename T> requires std::is_floating_point_v<T>
	template <typename ScalarType> requires std::is_arithmetic_v<ScalarType>
	TVector<T>& TVector<T>::operator/=(ScalarType Scale)
	{
		const T TypedScale = static_cast<T>(Scale);
		X /= TypedScale; Y /= TypedScale; Z /= TypedScale;
		return *this;
	}

	template <typename T> requires std::is_floating_point_v<T>
	T& TVector<T>::operator[](int32_t Index)
	{
		assert(Index >= 0 && Index < 3);
		return Index == 0 ? X : (Index == 1 ? Y : Z);
	}

	template <typename T> requires std::is_floating_point_v<T>
	const T& TVector<T>::operator[](int32_t Index) const
	{
		assert(Index >= 0 && Index < 3);
		return Index == 0 ? X : (Index == 1 ? Y : Z);
	}

	template <typename T> requires std::is_floating_point_v<T>
	TVector<T> TVector<T>::Cross(const TVector& V) const
	{
		return TVector(Y * V.Z - Z * V.Y, Z * V.X - X * V.Z, X * V.Y - Y * V.X);
	}

	template <typename T> requires std::is_floating_point_v<T>
	T TVector<T>::Dot(const TVector& V) const
	{
		return X * V.X + Y * V.Y + Z * V.Z;
	}

	template <typename T> requires std::is_floating_point_v<T>
	T TVector<T>::Size() const
	{
		return std::sqrt(SizeSquared());
	}

	template <typename T> requires std::is_floating_point_v<T>
	T TVector<T>::SizeSquared() const
	{
		return Dot(*this);
	}
}
