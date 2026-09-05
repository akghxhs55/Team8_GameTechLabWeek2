#pragma once

#include "UPrimitiveComponent.h"
#include "Runtime/Engine/UScene.h"

class UCylinderComp : public UPrimitiveComponent
{
	GENERATED_BODY()

protected:
	explicit UCylinderComp(int Axis) : Axis(Axis) {}

	void OnRegister(UScene& Scene) override;
	
private:
	int Axis;
};

inline void UCylinderComp::OnRegister(UScene& Scene)
{
	UPrimitiveComponent::OnRegister(Scene);

	SetMesh(Scene.GetRenderResourceLibrary().GetCylinderMesh(Axis));
	SetMaterial(Scene.GetRenderResourceLibrary().GetSimpleMaterial());
}
