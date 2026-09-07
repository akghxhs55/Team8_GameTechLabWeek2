#include "FEditorApplication.h"

void FEditorApplication::Initialize_ImguiWin32DX11(HWND& Window, ID3D11Device* Device, ID3D11DeviceContext* Context)
{
	ImguiManager.Initialize_ImplWin32DX11(Window, Device, Context);
}

void FEditorApplication::Initialize_Runtime(FRenderResourceLibrary* RendererLibrary, USceneManager* SceneManager, FRenderView* RenderView)
{
	this->RenderView = RenderView;
	this->SceneManager = SceneManager;
	Editor.Initialize(RendererLibrary, SceneManager);
	Editor.AddViewport();
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
	EditorViewportWindow.Process(Editor);
	PropertyWindow.Process(Editor);
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
