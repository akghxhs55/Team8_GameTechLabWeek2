#include <Windows.h>

#include "Runtime/Core/PointerTypes.h"
#include "Runtime/Core/TArray.h"
#include "Runtime/Core/FString.h"
#include "Runtime/Rendering/FRenderer.h"
#include <filesystem>

namespace
{
	constexpr LPCWSTR WindowName = L"My Engine";

	HWND CreateWindowHandle(HINSTANCE Instance);
	bool ProcessWindowMessage();
	FWString GetExecutableDirectory();

	// TODO: 좀 더 잘 된 팩토리 구현
	TSharedPtr<FMesh> CreateCubeMesh(FRenderer& Renderer, FVector Location, FVector Rotation, FVector Scale)
	{
		// TODO: Rotation 적용
		const auto MakePosition = [Location, Scale](float X, float Y, float Z) -> FVector3f
			{
				return {
					static_cast<float>(Location.X + X * Scale.X),
					static_cast<float>(Location.Y + Y * Scale.Y),
					static_cast<float>(Location.Z + Z * Scale.Z)
				};
			};

		TArray<FVertexPositionColor> Vertices = {
			{ MakePosition(-0.5, -0.5, -0.5), FVector3f(0.0, 0.0, 0.0) },
			{ MakePosition(0.5, -0.5, -0.5), FVector3f(1.0, 0.0, 0.0) },
			{ MakePosition(0.5,  0.5, -0.5), FVector3f(1.0, 1.0, 0.0) },
			{ MakePosition(-0.5,  0.5, -0.5), FVector3f(0.0, 1.0, 0.0) },
			{ MakePosition(-0.5, -0.5,  0.5), FVector3f(0.0, 0.0, 1.0) },
			{ MakePosition(0.5, -0.5,  0.5), FVector3f(1.0, 0.0, 1.0) },
			{ MakePosition(0.5,  0.5,  0.5), FVector3f(1.0, 1.0, 1.0) },
			{ MakePosition(-0.5,  0.5,  0.5), FVector3f(0.0, 1.0, 1.0) },
		};

		const TArray<uint32> Indices = {
			0, 2, 1, 0, 3, 2, // -Z
			4, 5, 6, 4, 6, 7, // +Z
			0, 1, 5, 0, 5, 4, // -Y
			3, 7, 6, 3, 6, 2, // +Y
			0, 4, 7, 0, 7, 3, // -X
			1, 2, 6, 1, 6, 5, // +X
		};

		FMeshDesc MeshDesc{
			.VertexLayout = EVertexLayout::PositionColor,
			.VertexData = Vertices.data(),
			.VertexDataSize = static_cast<uint32>(sizeof(FVertexPositionColor) * Vertices.size()),
			.VertexStride = static_cast<uint32>(sizeof(FVertexPositionColor)),
			.VertexCount = static_cast<uint32>(Vertices.size()),
			.IndexData = Indices.data(),
			.IndexDataSize = static_cast<uint32>(sizeof(uint32) * Indices.size()),
			.IndexCount = static_cast<uint32>(Indices.size()),
		};

		return Renderer.CreateMesh(MeshDesc);
	}

	TSharedPtr<FMaterial> CreateSimpleMaterial(FRenderer& Renderer)
	{
		FWString Path = GetExecutableDirectory();

		FMaterialDesc Desc = {
			.VertexShaderFileName = Path + L"/Shader/ExampleVS.cso",
			.PixelShaderFileName = Path + L"/Shader/ExamplePS.cso",
			.VertexLayout = EVertexLayout::PositionColor,
		};
		return Renderer.CreateMaterial(Desc);
	}
}

int WINAPI wWinMain(
    _In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR lpCmdLine,
    _In_ int nShowCmd) 
{
	HWND Window = CreateWindowHandle(hInstance);
	if (!Window)
	{
		return -1;
	}

	ShowWindow(Window, nShowCmd);

	FRenderer Renderer;
	if (!Renderer.Initialize(Window))
	{
		return -1;
	}

	TSharedPtr<FMesh> CubeMesh = CreateCubeMesh(Renderer, FVector(0.0, 0.0, 0.0), FVector(0.0, 0.0, 0.0), FVector(0.3, 0.3, 0.3));
	TSharedPtr<FMaterial> CubeMaterial = CreateSimpleMaterial(Renderer);

	bool bQuit = false;
    while (!bQuit)
    {
		if (!ProcessWindowMessage())
		{
			bQuit = true;
			break;
		}

        // UpdateCamera();

		Renderer.BeginFrame();
		
    	Renderer.Draw(*CubeMesh, *CubeMaterial);

		Renderer.SwapBuffer();
    }

	Renderer.Shutdown();

	return 0;
}

namespace
{
	HWND CreateWindowHandle(HINSTANCE Instance)
	{
		WNDCLASS WindowClass{};
		WindowClass.lpfnWndProc =
			[](HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) -> LRESULT
			{
				switch (uMsg)
				{
				case WM_DESTROY:
					PostQuitMessage(0);
					break;
				default:
					return DefWindowProc(hWnd, uMsg, wParam, lParam);
				}
				return 0;
			};
		WindowClass.hInstance = Instance;
		WindowClass.lpszClassName = L"MyEngine";

		if (!RegisterClass(&WindowClass) && GetLastError() != ERROR_CLASS_ALREADY_EXISTS)
		{
			return nullptr;
		}

		HWND Window = CreateWindowExW(
			0,
			WindowClass.lpszClassName,
			WindowName,
			WS_POPUP | WS_OVERLAPPEDWINDOW,
			CW_USEDEFAULT, CW_USEDEFAULT, 800, 600,
			nullptr, nullptr, Instance, nullptr);

		return Window;
	}

	// 닫아야되면 false 반환
	bool ProcessWindowMessage()
	{
		MSG Message;
		while (PeekMessageW(&Message, nullptr, 0u, 0u, PM_REMOVE))
		{
			if (Message.message == WM_QUIT)
			{
				return false;
			}

			TranslateMessage(&Message);
			DispatchMessageW(&Message);
		}

		return true;
	}

	FWString GetExecutableDirectory()
	{
		wchar_t Buffer[256];
		GetModuleFileNameW(nullptr, Buffer, 256);
		return std::filesystem::path(Buffer).parent_path();
	}
}
