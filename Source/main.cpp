#include "Editor/Application/FEditorApplication.h"
#include "Runtime/Engine/UScene.h"
#include "Runtime/Engine/FRenderView.h"
#include "Runtime/CoreUObject/UObjectGlobals.h"
#include "Runtime/Input/FInputManager.h"
#include "Runtime/Engine/FTimeManager.h"
#include "Runtime/Engine/FRayCastingManager.h"
#include "Runtime/Engine/USceneManager.h"
#include "Runtime/Rendering/FRenderer.h"
#include "Runtime/Rendering/FRenderResourceLibrary.h"
#include "Runtime/Math/FVector2.h"
#include "Runtime/Math/FMatrix.h"
#include "Runtime/CoreUObject/UClass.h"
#include "ThirdParty/Imgui/imgui.h"
#include "ThirdParty/Imgui/imgui_internal.h"
#include "ThirdParty/Imgui/imgui_impl_dx11.h"
#include "ThirdParty/Imgui/imgui_impl_win32.h"
#include <Windows.h>
#include <windowsx.h>

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
bool bRequestNewScene = false;
bool bRequestSaveScene = false;
bool bRequestLoadScene = false;

namespace
{
	constexpr LPCWSTR WindowName = L"My Engine";

	HWND CreateWindowHandle(HINSTANCE Instance);
	bool ProcessWindowMessage();

	LRESULT CALLBACK WindowCallback(HWND Window, UINT Message, WPARAM WParam, LPARAM LParam);
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

	FInputManager::Get();

	FRenderer Renderer;
	if (!Renderer.Initialize(Window))
	{
		return -1;
	}
	FRenderView RenderView(Renderer);

	FRenderResourceLibrary RenderResources;
	if (!RenderResources.Initialize(Renderer))
	{
		return -1;
	}

	USceneManager tmp;
	tmp.resourceLibrary = &RenderResources;
	tmp.SetScene(NewObject<UScene>(RenderResources));


	FEditorApplication& EditorApp = FEditorApplication::Get();
	{
		ID3D11Device* Device = nullptr; ID3D11DeviceContext* Context = nullptr;
		Renderer.GetDeviceAndContext_ImplDX11(Device, Context);
		EditorApp.Initialize_ImguiWin32DX11(Window, Device, Context);
	}
	EditorApp.Initialize_Runtime(&RenderResources, &tmp, &RenderView);
	//FImguiManager& UIManager = FImguiManager::Get();
	//{
	//	ID3D11Device* Device = nullptr; ID3D11DeviceContext* Context = nullptr;
	//	Renderer.GetDeviceAndContext_ImplDX11(Device, Context);
	//	UIManager.Initialize_ImplWin32DX11(Window, Device, Context);
	//}



	// TODO: 임시 Scene 생성. 나중에 Scene 불러오고 편집하는 기능 구현
	//UScene* Scene = NewObject<UScene>(RenderResources);
	//UCubeComp* CubeComp = NewObject<UCubeComp>();
	//CubeComp->RelativeTransform.Location = FVector{ 1.0f, 1.0f, 0.0f };
	//CubeComp->RelativeTransform.Rotation = FQuaternion::FromEulerXYZDeg(FVector{ 0.5f, 0.5f, 0.5f });
	//CubeComp->RelativeTransform.Scale3D = FVector{ 0.5f, 0.5f, 0.5f };
	

	//해당 경로에 UUID 기록 성공
	/*USceneManager tmp;
	tmp.resourceLibrary = &RenderResources;
	tmp.SetScene(NewObject<UScene>(*tmp.resourceLibrary));
	tmp.currentScene->RegisterComponent(*CubeComp);

	UCylinderComp* CylinderCompX = NewObject<UCylinderComp>();
	CylinderCompX->RelativeTransform.Location = FVector{ 0.3f, 0.0f, 0.0f };
	CylinderCompX->RelativeTransform.Rotation = FQuaternion::FromEulerXYZDeg(FVector{ 0.0f, 0.0f, -90.0f });
	CylinderCompX->RelativeTransform.Scale3D = FVector{ 0.2f, 0.5f, 0.2f };
	tmp.currentScene->RegisterComponent(*CylinderCompX);

	UCylinderComp* CylinderCompY = NewObject<UCylinderComp>();
	CylinderCompY->RelativeTransform.Location = FVector{ 0.0f, 0.3f, 0.0f };
	CylinderCompY->RelativeTransform.Rotation = FQuaternion::FromEulerXYZDeg(FVector{ 0.0f, 0.0f, 0.0f });
	CylinderCompY->RelativeTransform.Scale3D = FVector{ 0.2f, 0.5f, 0.2f };
	tmp.currentScene->RegisterComponent(*CylinderCompY);

	UCylinderComp* CylinderCompZ = NewObject<UCylinderComp>();
	CylinderCompZ->RelativeTransform.Location = FVector{ 0.0f, 0.0f, 0.3f };
	CylinderCompZ->RelativeTransform.Rotation = FQuaternion::FromEulerXYZDeg(FVector{ -90.0f, 0.0f, 0.0f });
	CylinderCompZ->RelativeTransform.Scale3D = FVector{ 0.2f, 0.5f, 0.2f };
	tmp.currentScene->RegisterComponent(*CylinderCompZ);*/

	FCamera Camera{};
	Camera.Position = FVector(-3.0f, 3.0f, 2.0f);
	Camera.Pitch = -25.0f;
	Camera.Yaw = -45.0f;
	//Camera.Projection.ProjectionType = EProjectionType::Orthographic;

	static FCameraInputController CameraController;

	bool bQuit = false;
    while (!bQuit)
    {
		FTimeManager::Get().Update();
		FTimeManager::Get().Resume();

		if (!ProcessWindowMessage())
		{
			bQuit = true;
			break;
		}


		//CameraController.HandleMouseInput(Camera, FTimeManager::Get().GetDeltaTime(), FInputManager::Get().GetMouseDelta());
		FInputManager::Get().BeginFrame();

		EditorApp.Update(FTimeManager::Get().GetDeltaTime());

		Renderer.BeginFrame();

		EditorApp.Render();

		//ImGui_ImplDX11_NewFrame();
		//ImGui_ImplWin32_NewFrame();
		//ImGui::NewFrame();

		//// 여기
		//ImGui::ShowDemoWindow();

		//ImGui::Render();

		//ImGui_ImplDX11_RenderDrawData(
		//	ImGui::GetDrawData()
		//);// 마우스를 누른 첫 프레임만 피킹



			

		Renderer.SwapBuffer();
    }

	Renderer.Shutdown();

	return 0;
}

namespace
{
	// TODO: Resizing 처리
	HWND CreateWindowHandle(HINSTANCE Instance)
	{
		WNDCLASS WindowClass{};
		WindowClass.lpfnWndProc = WindowCallback;
			
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

	LRESULT CALLBACK WindowCallback(HWND Window, UINT Message, WPARAM WParam, LPARAM LParam)
	{
		if (LRESULT ImGuiResult = ImGui_ImplWin32_WndProcHandler(Window, Message, WParam, LParam)) // imgui의 프레임 스냅샷 상태를 갱신
			return ImGuiResult; // ImGuiResult != 0인 경우: 상태가 DefWindowProcW() 함수 동작을 오버라이드해야 하는 경우

		const FVector2 MousePos{
			static_cast<float>(GET_X_LPARAM(LParam)),
			static_cast<float>(GET_Y_LPARAM(LParam))
		};

		switch (Message)
		{
		case WM_DESTROY:
			PostQuitMessage(0);
			break;

		case WM_SIZE:
		{
			if (WParam != SIZE_MINIMIZED)
			{
				UINT Width = LOWORD(LParam);
				UINT Height = HIWORD(LParam);

				//Renderer.Resize(Width, Height);
			}

			break;
		}

		case WM_LBUTTONDOWN:
			FInputManager::Get().OnMouseButtonDown(EMouseButton::Left, MousePos);
			SetCapture(Window);
			break;

		case WM_RBUTTONDOWN:
			FInputManager::Get().OnMouseButtonDown(EMouseButton::Right, MousePos);
			SetCapture(Window);
			break;

		case WM_MBUTTONDOWN:
			FInputManager::Get().OnMouseButtonDown(EMouseButton::Middle, MousePos);
			SetCapture(Window);
			break;

		case WM_LBUTTONUP:
			FInputManager::Get().OnMouseButtonUp(EMouseButton::Left, MousePos);
			ReleaseCapture();
			break;

		case WM_RBUTTONUP:
			FInputManager::Get().OnMouseButtonUp(EMouseButton::Right, MousePos);
			ReleaseCapture();
			break;

		case WM_MBUTTONUP:
			FInputManager::Get().OnMouseButtonUp(EMouseButton::Middle, MousePos);
			ReleaseCapture();
			break;

		case WM_MOUSEMOVE:
			FInputManager::Get().OnMouseMove(MousePos);
			break;

		case WM_CAPTURECHANGED:
		case WM_CANCELMODE:
		case WM_KILLFOCUS:
			ReleaseCapture();
			break;
		case WM_KEYDOWN:
			switch (WParam)
			{
			case VK_F5: bRequestSaveScene = true; break;
			case VK_F6: bRequestLoadScene = true; break;
			case VK_F7: bRequestNewScene = true; break;
			}
			break;
		default:
			return DefWindowProc(Window, Message, WParam, LParam);
		}

		return 0;
	}
}
