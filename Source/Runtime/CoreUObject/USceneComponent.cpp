#include "USceneComponent.h"

void USceneComponent::Serialize()
{
	UObject::Serialize();
}

FTransform USceneComponent::GetRelativeTransform()
{
    return RelativeTransform;
}

void USceneComponent::SetRelativeTransform(FTransform RelativeTransform)
{
    this->RelativeTransform = RelativeTransform;
}
