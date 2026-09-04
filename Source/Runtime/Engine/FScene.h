#pragma once

#include "Runtime/CoreUObject/USceneComponent.h"
#include "Runtime/Core/Types.h"
#include "Runtime/Core/TArray.h"

struct FScene
{
	uint32 Version;
	uint32 NextUUID;
	TArray<USceneComponent*> Components;
};
