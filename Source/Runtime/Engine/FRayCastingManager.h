#pragma once

#include "Runtime/Math/FVector.h"
#include "Runtime/Core/TArray.h"
#include "Runtime/Math/FVector2.h"
#include "Runtime/Engine/FCamera.h"
#include <cmath>
struct FRay
{
	FVector Origin;
	FVector Direction;
};

class FRayCastingManager
{

private:
    float MouseX, MouseY;


public:
    static FRayCastingManager& Get()
    {
        static FRayCastingManager instance;

        return instance;
    }

    FRay CreateRayFromScreenPosition(FCamera* camera, FVector2& viewportSize);
    template<typename T>
    bool RayIntersectsMeshes(FCamera* camera, TArray<T*>& components, T*& hitComponent, FVector& outImpactPoint, FVector2& viewportSize);
};


	


    static bool RayIntersectsTriangle(
        const FRay& ray,
        const FVector& a,
        const FVector& b,
        const FVector& c,
        float& outT)
    {
        constexpr float Epsilon = 0.000001f;

        const FVector edge1 = b - a;
        const FVector edge2 = c - a;

        // Ray direction × triangle edge
        const FVector pVector = ray.Direction.Cross(edge2);
        const float determinant = edge1.Dot(pVector);

        // 레이와 삼각형 평면이 평행함
        if (std::fabs(determinant) < Epsilon)
        {
            return false;
        }

        const float inverseDeterminant = 1.0f / determinant;

        // Barycentric u 계산
        const FVector tVector = ray.Origin - a;
        const float u = tVector.Dot(pVector) * inverseDeterminant;

        if (u < 0.0f || u > 1.0f)
        {
            return false;
        }

        // Barycentric v 계산
        const FVector qVector = tVector.Cross(edge1);
        const float v = ray.Direction.Dot(qVector) * inverseDeterminant;

        if (v < 0.0f || u + v > 1.0f)
        {
            return false;
        }

        // 레이 시작점으로부터 교점까지의 거리
        outT = edge2.Dot(qVector) * inverseDeterminant;

        // t가 음수면 카메라/레이 시작점 뒤에 있는 삼각형
        return outT > Epsilon;
    }


