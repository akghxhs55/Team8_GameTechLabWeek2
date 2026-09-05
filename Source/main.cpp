#include "Runtime/Engine/UScene.h"
#include "Runtime/Engine/FViewportCamera.h"
#include "Runtime/CoreUObject/UObjectGlobals.h"
#include "Runtime/CoreUObject/UCubeComp.h"
#include "Runtime/CoreUObject/UCylinderComp.h"
#include "Runtime/Input/FCameraInputController.h"
#include "Runtime/Input/FInputManager.h"
#include "Runtime/Rendering/FRenderer.h"
#include "Runtime/Rendering/FRenderResourceLibrary.h"
#include "Runtime/Math/FVector.h"
#include "Runtime/Math/FVector2.h"
#include "Runtime/Math/FMatrix.h"
#include <Windows.h>
#include <windowsx.h>

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
	CubeComp->RelativeTransform.Location = FVector{ 1.0f, 1.0f, 0.0f };
	CubeComp->RelativeTransform.Rotation = FQuaternion::FromEulerXYZDeg(FVector{ 0.5f, 0.5f, 0.5f });
	CubeComp->RelativeTransform.Scale3D = FVector{ 0.5f, 0.5f, 0.5f };
	Scene->RegisterComponent(*CubeComp);

	UCylinderComp* CylinderCompX = NewObject<UCylinderComp>(0);
	CylinderCompX->RelativeTransform.Location = FVector{ 0.3f, 0.0f, 0.0f };
	CylinderCompX->RelativeTransform.Rotation = FQuaternion::FromEulerXYZDeg(FVector{ 0.0f, 0.0f, -90.0f });
	CylinderCompX->RelativeTransform.Scale3D = FVector{ 0.2f, 0.5f, 0.2f };
	Scene->RegisterComponent(*CylinderCompX);

	UCylinderComp* CylinderCompY = NewObject<UCylinderComp>(1);
	CylinderCompY->RelativeTransform.Location = FVector{ 0.0f, 0.3f, 0.0f };
	CylinderCompY->RelativeTransform.Rotation = FQuaternion::FromEulerXYZDeg(FVector{ 0.0f, 0.0f, 0.0f });
	CylinderCompY->RelativeTransform.Scale3D = FVector{ 0.2f, 0.5f, 0.2f };
	Scene->RegisterComponent(*CylinderCompY);

	UCylinderComp* CylinderCompZ = NewObject<UCylinderComp>(2);
	CylinderCompZ->RelativeTransform.Location = FVector{ 0.0f, 0.0f, 0.3f };
	CylinderCompZ->RelativeTransform.Rotation = FQuaternion::FromEulerXYZDeg(FVector{ -90.0f, 0.0f, 0.0f });
	CylinderCompZ->RelativeTransform.Scale3D = FVector{ 0.2f, 0.5f, 0.2f };
	Scene->RegisterComponent(*CylinderCompZ);

	FViewportCamera Camera{};
	Camera.Position = FVector(-3.0f, 3.0f, 2.0f);
	Camera.Pitch = -25.0f;
	Camera.Yaw = -45.0f;
	//Camera.Projection.ProjectionType = EProjectionType::Orthographic;

	// TODO: 추상화
	static FCameraInputController CameraController;

	// TODO: DeltaTime 계산
	bool bQuit = false;
    while (!bQuit)
    {
		if (!ProcessWindowMessage())
		{
			bQuit = true;
			break;
		}

		CameraController.HandleMouseInput(Camera, FInputManager::Get().GetMouseDelta());
		FInputManager::Get().Update();
		CameraController.UpdateKeyInput(Camera, 1.0f / 60.0f);

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
	// TODO: Resizing 처리
	// TODO: 마우스 입력 추상화. FInputManager에 합칠 수 있을까?
	// TODO: 마우스가 화면 밖에 나갈 때 처리가 잘 안 됨
	HWND CreateWindowHandle(HINSTANCE Instance)
	{
		static FVector2 PrevMousePos{ 0.0f, 0.0f };
		static bool bMousePressed = false;

		WNDCLASS WindowClass{};
		WindowClass.lpfnWndProc =
			[](HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) -> LRESULT
			{
				switch (uMsg)
				{
				case WM_DESTROY:
					PostQuitMessage(0);
					break;

				case WM_RBUTTONDOWN:
					bMousePressed = true;
					break;

				case WM_RBUTTONUP:
					bMousePressed = false;
					break;

				case WM_MOUSEMOVE:
					if (bMousePressed)
					{
						const FVector2 MousePos{
							static_cast<float>(GET_X_LPARAM(lParam)),
							static_cast<float>(GET_Y_LPARAM(lParam))
						};
						const FVector2 MouseDelta = MousePos - PrevMousePos;
						FInputManager::Get().AddMouseInput(MouseDelta);
					}
					PrevMousePos = FVector2{
						static_cast<float>(GET_X_LPARAM(lParam)),
						static_cast<float>(GET_Y_LPARAM(lParam))
					};
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
			CW_USEDEFAULT, CW_USEDEFAULT, 800, 800,
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
