#include "USceneComponent.h"
#include "ThirdParty/Json/json.hpp"

﻿#include "USceneComponent.h"

FTransform USceneComponent::GetRelativeTransform()
{
    return RelativeTransform;
}

void USceneComponent::SetRelativeTransform(FTransform RelativeTransform)
{
    this->RelativeTransform = RelativeTransform;
#include "USceneComponent.h"

void USceneComponent::Serialize() const
{



}
