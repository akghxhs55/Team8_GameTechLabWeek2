#pragma once

#include "UObject.h"
#include "Runtime/Geometry/FTransform.h"

class UScene;

class USceneComponent : public UObject
{
	GENERATED_BODY()

	friend UScene;

public:
	FTransform RelativeTransform;

protected:
	USceneComponent() = default;

	virtual void OnRegister(UScene& Scene) {}
	virtual void OnUnregister(UScene& Scene) {}
public:
	FTransform GetRelativeTransform();
	void SetRelativeTransform(FTransform RelativeTransform);
	FTransform GetGlobalTransform();
	void SetRelativeTransformFromGlobal(FTransform GlobalTransform);
};
