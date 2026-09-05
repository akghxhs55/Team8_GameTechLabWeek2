#pragma once

#include "USceneComponent.h"
#include "Runtime/Core/PointerTypes.h"
#include "Runtime/Rendering/FMaterial.h"
#include "Runtime/Rendering/FMesh.h"

class UPrimitiveComponent : public USceneComponent
{
public:
	void SetMesh(TSharedPtr<FMesh> Mesh) { PrimitiveMesh = std::move(Mesh); }
	[[nodiscard]] TSharedPtr<FMesh> GetMesh() const { return PrimitiveMesh; }

	void SetMaterial(TSharedPtr<FMaterial> Material) { PrimitiveMaterial = std::move(Material); }
	[[nodiscard]] TSharedPtr<FMaterial> GetMaterial() const { return PrimitiveMaterial; }

protected:
	UPrimitiveComponent() = default;

private:
	TSharedPtr<FMesh> PrimitiveMesh;
	TSharedPtr<FMaterial> PrimitiveMaterial;
};
