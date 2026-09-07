#pragma once
#include "Editor/Core/FEditor.h"
#include "Editor/UI/Imgui/FImguiManager.h"
#include "Editor/UI/Imgui/FImguiPropertyWindow.h"
#include "Editor/UI/Imgui/FImguiEditorViewportWindow.h"
#include "Runtime/Engine/FRenderView.h"
#include "Runtime/Input/FCameraInputController.h"

class FEditorApplication final {
	FEditor Editor;
	USceneManager* SceneManager = nullptr;

	FImguiManager ImguiManager;

	FImguiEditorViewportWindow EditorViewportWindow;
	FImguiPropertyWindow PropertyWindow;

	FCameraInputController CameraController;

	FRenderView* RenderView = nullptr;
public:
	static FEditorApplication& Get()
	{
		static FEditorApplication Instance;
		return Instance;
	}

	//복사 연산자와 복사 대입 연산자 삭제
	FEditorApplication(const FEditorApplication&) = delete;
	FEditorApplication& operator=(const FEditorApplication&) = delete;

	//이동 연산자와 이동 대입연산자 삭제
	FEditorApplication(FEditorApplication&&) = delete;
	FEditorApplication& operator=(FEditorApplication&&) = delete;

	void Initialize_ImguiWin32DX11(HWND& Window, ID3D11Device* Device, ID3D11DeviceContext* Context);
	void Initialize_Runtime(FRenderResourceLibrary* RendererLibrary, USceneManager* SceneManager, FRenderView* RenderView); // TODO: RendererLibrary의존성 사라져야 함
	bool CheckSceneExistsAndInitializeIfNotExists(const FString& path = "");
	void Update(float DeltaTime);
	void Render();
private:
	FEditorApplication() = default;
	~FEditorApplication() = default;
	void BeginFrame();
	void Tick(float DeltaTime);
};
