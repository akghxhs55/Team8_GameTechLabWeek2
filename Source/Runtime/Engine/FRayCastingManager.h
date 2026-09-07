#pragma once

#include "Runtime/Math/FVector.h"
class UCamera;

struct FRay
{
	FVector Origin;
	FVector Direction;
};

class FRayCastingManager
{

public:
	static FRayCastingManager& Get()
	{
		static FRayCastingManager instance;

		return instance;
	}

	FRay CreateRayFromScreenPosition(UCamera* camera);







};