#include "USceneComponent.h"

FTransform USceneComponent::GetRelativeTransform()
{
    return RelativeTransform;
}

void USceneComponent::SetRelativeTransform(FTransform RelativeTransform)
{
    this->RelativeTransform = RelativeTransform;
}
