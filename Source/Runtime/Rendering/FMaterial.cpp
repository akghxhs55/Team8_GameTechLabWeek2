#include "FMaterial.h"

#include "FRenderer.h"
#include "Vertices.h"
#include "Runtime/Core/FString.h"
#include "Runtime/Core/PointerTypes.h"
#include <d3d11.h>

void FMaterial::BindResources(ID3D11DeviceContext* Context) const
{
	if (!Context)
	{
		return;
	}
}
