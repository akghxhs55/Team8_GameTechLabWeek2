#include "FEditorApplication.h"

#include "Runtime/CoreUObject/UCubeComp.h"
#include "Runtime/CoreUObject/UCylinderComp.h"
#include "Runtime/CoreUObject/UObjectGlobals.h"

void FEditorApplication::Initialize_ImguiWin32DX11(HWND& Window, ID3D11Device* Device, ID3D11DeviceContext* Context)
{
	ImguiManager.Initialize_ImplWin32DX11(Window, Device, Context);
}

void FEditorApplication::Initialize_Runtime(FRenderResourceLibrary* RendererLibrary, USceneManager* SceneManager, FRenderView* RenderView)
{
	this->RenderView = RenderView;
	this->SceneManager = SceneManager;

	Editor.Initialize(RendererLibrary, SceneManager);

	UCubeComp* CubeComp = NewObject<UCubeComp>();
	CubeComp->RelativeTransform.Location = FVector{ 1.0f, 1.0f, 0.0f };
	CubeComp->RelativeTransform.Rotation = FQuaternion::FromEulerXYZDeg(FVector{ 0.5f, 0.5f, 0.5f });
	CubeComp->RelativeTransform.Scale3D = FVector{ 0.5f, 0.5f, 0.5f };
	SceneManager->currentScene->RegisterComponent(*CubeComp);

	UCylinderComp* CylinderCompX = NewObject<UCylinderComp>();
	CylinderCompX->RelativeTransform.Location = FVector{ 0.3f, 0.0f, 0.0f };
	CylinderCompX->RelativeTransform.Rotation = FQuaternion::FromEulerXYZDeg(FVector{ 0.0f, 0.0f, -90.0f });
	CylinderCompX->RelativeTransform.Scale3D = FVector{ 0.2f, 0.5f, 0.2f };
	SceneManager->currentScene->RegisterComponent(*CylinderCompX);

	UCylinderComp* CylinderCompY = NewObject<UCylinderComp>();
	CylinderCompY->RelativeTransform.Location = FVector{ 0.0f, 0.3f, 0.0f };
	CylinderCompY->RelativeTransform.Rotation = FQuaternion::FromEulerXYZDeg(FVector{ 0.0f, 0.0f, 0.0f });
	CylinderCompY->RelativeTransform.Scale3D = FVector{ 0.2f, 0.5f, 0.2f };
	SceneManager->currentScene->RegisterComponent(*CylinderCompY);

	UCylinderComp* CylinderCompZ = NewObject<UCylinderComp>();
	CylinderCompZ->RelativeTransform.Location = FVector{ 0.0f, 0.0f, 0.3f };
	CylinderCompZ->RelativeTransform.Rotation = FQuaternion::FromEulerXYZDeg(FVector{ -90.0f, 0.0f, 0.0f });
	CylinderCompZ->RelativeTransform.Scale3D = FVector{ 0.2f, 0.5f, 0.2f };
	SceneManager->currentScene->RegisterComponent(*CylinderCompZ);

	Editor.SelectObject(CubeComp);

	FEditorViewport Viewport;
	Viewport.ViewportCamera.Position = FVector{ -3.0f, 3.0f, 2.0f };
	Viewport.ViewportCamera.Pitch = -25.0f;
	Viewport.ViewportCamera.Yaw = -45.0f;
	Editor.AddViewport(Viewport);
	//Editor.LoadScene("");
}

/// <summary>
/// return value: if scene is pre-existing, returns true
/// if scene was not existing, returns false
/// </summary>
/// <param name="path"></param>
/// <returns></returns>
bool FEditorApplication::CheckSceneExistsAndInitializeIfNotExists(const FString& path)
{
	if (Editor.CheckSceneExists())
		return true;
	else {
		if (path == "")
			Editor.NewScene();
		else
			Editor.LoadScene(path);
		return false;
	}
}

void FEditorApplication::Update(float DeltaTime)
{
	BeginFrame();
	Tick(DeltaTime);
	Render();
}

void FEditorApplication::BeginFrame()
{
	ImguiManager.NewFrame();
}

void FEditorApplication::Tick(float DeltaTime)
{
	ControlPanelWindow.Process(Editor);
	EditorViewportWindow.Process(Editor);
	PropertyWindow.Process(Editor);

	if (FEditorViewport* ActiveViewport = Editor.GetActiveViewport())
	{
		FCamera& Camera = ActiveViewport->ViewportCamera;
		CameraController.UpdateMouseInput(Camera);
		CameraController.UpdateKeyInput(Camera, DeltaTime);
	}
}

void FEditorApplication::Render()
{
	const TArray<FEditorViewport>& EditorViewports = Editor.GetViewports();

	for (auto& EditorViewport : EditorViewports) {
		for (auto& PrimitiveComponent : SceneManager->currentScene->GetPrimitiveComponents())
			RenderView->Render(EditorViewport.ViewportCamera, EditorViewport.TopLeft,
				EditorViewport.Length, PrimitiveComponent);
	}
	// TODO: render HighLight for selected object
	// TODO: render Gizmo
	ImguiManager.RenderUI();
}
