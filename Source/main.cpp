#include <Windows.h>

#include "Runtime/Engine/UScene.h"
#include "Runtime/Engine/FViewportCamera.h"
#include "Runtime/CoreUObject/UObjectGlobals.h"
#include "Runtime/CoreUObject/UCubeComp.h"
#include "Runtime/Input/FInputManager.h"
#include "Runtime/Rendering/FRenderer.h"
#include "Runtime/Rendering/FRenderResourceLibrary.h"
#include "Runtime/Math/FVector.h"
#include "Runtime/Math/FMatrix.h"

namespace
{
	constexpr LPCWSTR WindowName = L"My Engine";

	HWND CreateWindowHandle(HINSTANCE Instance);
	bool ProcessWindowMessage();
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

	FRenderResourceLibrary RenderResources;
	if (!RenderResources.Initialize(Renderer))
	{
		return -1;
	}

	// TODO: 임시 Scene 생성. 나중에 Scene 불러오고 편집하는 기능 구현
	UScene* Scene = NewObject<UScene>(RenderResources);
	UCubeComp* CubeComp = NewObject<UCubeComp>();
	CubeComp->RelativeTransform.Location = FVector(0.0, 0.0, 0.0);
	CubeComp->RelativeTransform.Rotation = FQuaternion::FromEulerXYZDeg(FVector(0.5, 0.5, 0.5));
	CubeComp->RelativeTransform.Scale3D = FVector(0.5, 0.5, 0.5);
	Scene->RegisterComponent(*CubeComp);

	FViewportCamera Camera{};
	Camera.Position = FVector(-3.0f, -3.0f, 2.0f);
	Camera.Pitch = -25.0f;
	Camera.Yaw = 45.0f;

	bool bQuit = false;
    while (!bQuit)
    {
		if (!ProcessWindowMessage())
		{
			bQuit = true;
			break;
		}

		FInputManager::Get().Update();
		if (FInputManager::Get().IsKeyPressed(VK_LEFT))
		{
			Camera.Position.Y -= 1;
		}
		if (FInputManager::Get().IsKeyPressed(VK_RIGHT))
		{
			Camera.Position.Y += 1;
		}
		if (FInputManager::Get().IsKeyPressed(VK_UP))
		{
			Camera.Position.X += 1;
		}
		if (FInputManager::Get().IsKeyPressed(VK_DOWN))
		{
			Camera.Position.X -= 1;
		}

		const FMatrix VP = Camera.CreateViewProjectionMatrix();

		Renderer.BeginFrame();
		
		for (const auto& Component : Scene->GetPrimitiveComponents())
		{
			Renderer.UpdateObjectConstants({ Component->RelativeTransform.ToMatrix() * VP });
			Renderer.Draw(*Component->GetMesh(), *Component->GetMaterial());
		}

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
}
