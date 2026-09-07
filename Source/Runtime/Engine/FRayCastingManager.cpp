#include "FRayCastingManager.h"
#include "Runtime/Input/FInputManager.h"
#include "Runtime/Math/FMatrix.h"
#include "Runtime/Rendering/FMesh.h"
#include <limits>
#include "Runtime/CoreUObject/UPrimitiveComponent.h"

FRay FRayCastingManager::CreateRayFromScreenPosition(FCamera* camera, FVector2& viewportSize)
{

    int32 viewportWidth = viewportSize.X, viewportHeight = viewportSize.Y;

    FMatrix invVP;
    camera->CreateViewProjectionMatrix().Inverse(invVP);

    
    const float screenNdcX = (MouseX / viewportWidth) * 2.0f - 1.0f;
    const float screenNdcY = 1.0f - (MouseY / viewportHeight) * 2.0f;

    // 이 엔진의 투영 행렬:
    // X = depth, Y = screen horizontal, Z = screen vertical
    FVector nearClip{ 0.0f, screenNdcX, screenNdcY };
    FVector farClip{ 1.0f, screenNdcX, screenNdcY };

    const FVector nearWorld = invVP.TransformPointRow(nearClip);
    const FVector farWorld = invVP.TransformPointRow(farClip);

    FRay ray;
    ray.Origin = nearWorld;
    FVector dir = farWorld - nearWorld;
    ray.Direction = dir / dir.Size();
    return ray;
}


template<typename T>
inline bool FRayCastingManager::RayIntersectsMeshes(
    FCamera* camera,
    TArray<T*>& components,
    T*& hitComponent,
    FVector& outImpactPoint,
    FVector2& viewportSize)
{
    hitComponent = nullptr;

    MouseX = FInputManager::Get().GetMousePosition().X;
    MouseY = FInputManager::Get().GetMousePosition().Y;

    const FRay ray = CreateRayFromScreenPosition(camera, viewportSize);

    float closestHit = (std::numeric_limits<float>::max)();
    T* closestComponent = nullptr;
    FVector closestImpactPoint;

    for (T* component : components)
    {
        if (!component)
        {
            continue;
        }

        auto mesh = component->GetMesh();
        if (!mesh)
        {
            continue;
        }

        const auto& positions = mesh->GetPositions();
        const auto& indices = mesh->GetIndices();

        if (positions.size() < 3)
        {
            continue;
        }

        // GetRelativeTransform()은 FTransform이므로 이 줄은 제거
        // FMatrix worldTransform = component->GetRelativeTransform();

        // TransformPointRow가 현재 const 함수가 아니므로 const를 붙이지 않음
        FMatrix world = component->GetModelMatrix();

        const uint32 elementCount = mesh->HasIndices()
            ? static_cast<uint32>(indices.size())
            : static_cast<uint32>(positions.size());

        for (uint32 i = 0; i + 2 < elementCount; i += 3)
        {
            const uint32 i0 = mesh->HasIndices() ? indices[i] : i;
            const uint32 i1 = mesh->HasIndices() ? indices[i + 1] : i + 1;
            const uint32 i2 = mesh->HasIndices() ? indices[i + 2] : i + 2;

            // 잘못된 인덱스 방어
            if (i0 >= positions.size() ||
                i1 >= positions.size() ||
                i2 >= positions.size())
            {
                continue;
            }

            FVector a = positions[i0];
            FVector b = positions[i1];
            FVector c = positions[i2];

            // Local space → World space
            a = world.TransformPointRow(a);
            b = world.TransformPointRow(b);
            c = world.TransformPointRow(c);

            float hitT = 0.0f;

            if (RayIntersectsTriangle(ray, a, b, c, hitT) &&
                hitT < closestHit)
            {
                closestHit = hitT;
                closestComponent = component;
                closestImpactPoint = ray.Origin + ray.Direction * hitT;
            }
        }
    }


    if (!closestComponent)
    {
        return false;
    }

    hitComponent = closestComponent;
    outImpactPoint = closestImpactPoint;
    return true;
}


template bool FRayCastingManager::RayIntersectsMeshes<UPrimitiveComponent>(
    FCamera* camera,
    TArray<UPrimitiveComponent*>& components,
    UPrimitiveComponent*& hitComponent,
    FVector& outImpactPoint,
    FVector2& viewportSize
);