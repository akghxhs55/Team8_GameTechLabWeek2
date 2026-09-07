#pragma once
#include "Runtime/CoreUObject/UObject.h"
#include "Runtime/Engine/USceneManager.h"
#include "Runtime/Core/TArray.h"
#include "Editor/EditorViewport/FEditorViewport.h"
#include <memory>

class FEditor final {
private:
	FRenderResourceLibrary* RendererLibrary; // TODO: 없어도 되게(렌더러 구현 숨김)

	USceneManager* SceneManager; // 씬을 다중으로 가질 수 있도록 구조개선 가능-이경우 에디터쪽에 클래스를 추가해 씬과 FEditorViewport들을 연관
	TArray<FEditorViewport> EditorViewports;
	UObject* SelectedObject = nullptr;
public:
	void Initialize(FRenderResourceLibrary* RendererLibrary, USceneManager* SceneManager);
	
	void NewScene();
	void SaveScene(const FString& path);
	void LoadScene(const FString& path); // TODO: 실제 동작을 하는 코드로 갱신
	bool CheckSceneExists();
	
	void AddViewport();
	void DeleteViewport(int32 IndexOfViewport);
	
	bool SelectObject(UObject* Object);
	void UnSelectObject();
	UObject* GetSelectedObject();

	const TArray<FEditorViewport>& GetViewports() const;
};