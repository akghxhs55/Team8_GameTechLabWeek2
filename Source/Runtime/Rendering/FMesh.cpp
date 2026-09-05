#include "FMesh.h"

#include "FRenderer.h"
#include "Vertices.h"
#include "Runtime/Core/PointerTypes.h"
#include "Runtime/Math/VectorTypes.h"
#include <d3d11.h>
#include <wrl/client.h>

void FMesh::BindResources(ID3D11DeviceContext* Context) const
{
	if (!Context)
	{
		return;
	}

	constexpr UINT Offset = 0;

	Context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	Context->IASetVertexBuffers(0, 1, VertexBuffer.GetAddressOf(), &VertexStride, &Offset);
	Context->IASetIndexBuffer(IndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
}

