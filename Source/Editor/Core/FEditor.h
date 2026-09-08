#pragma once

#include "Runtime/CoreUObject/UObject.h"
#include "Runtime/Engine/USceneManager.h"
#include "Runtime/Core/TArray.h"
#include "Runtime/Core/IntTypes.h"
#include "Editor/EditorViewport/FEditorViewport.h"
#include "Editor/Gizmo/FGizmo.h"
#include "Editor/Grid/FGrid.h"

enum class EEditorPrimitiveType : uint8
{
	Cube,
	Cylinder,
	Sphere,   // TODO: USphereComp / 스피어 메시 미구현 - 현재 스폰 불가
};

class FEditor final {
public:
	FVector SelectedLocation;
	FVector SelectedRotationDeg;
	FVector SelectedScale3D;

public:
	void Initialize(FRenderResourceLibrary* RendererLibrary, USceneManager* SceneManager);
	
	void Process();

	void NewScene();
	void SaveScene(const FString& Path);
	void LoadScene(const FString& Path); // TODO: 실제 동작을 하는 코드로 갱신
	bool CheckSceneExists();
	
	void AddViewport(FEditorViewport Viewport);
	void DeleteViewport(int32 IndexOfViewport);
	FEditorViewport* GetActiveViewport(); // TODO: 임시로 0번 반환
	
	bool SelectObject(UObject* Object);
	void UnSelectObject();
	UObject* GetSelectedObject();
	[[nodiscard]] bool ObjectSelected() const { return SelectedObject != nullptr; }

	[[nodiscard]] const TArray<FEditorViewport>& GetViewports() const;
	UPrimitiveComponent* SpawnPrimitive(EEditorPrimitiveType Type);
	// 피킹 등에서 현재 씬의 렌더링 대상 컴포넌트가 필요할 때 사용
	[[nodiscard]] TArray<UPrimitiveComponent*> GetPrimitiveComponents() const;
	FGizmo& GetGizmo() { return Gizmo; }
	FGrid& GetGrid() { return Grid; }
private:
	FRenderResourceLibrary* RendererLibrary = nullptr;
	USceneManager* SceneManager = nullptr; // 씬을 다중으로 가질 수 있도록 구조개선 가능-이경우 에디터쪽에 클래스를 추가해 씬과 FEditorViewport들을 연관
	TArray<FEditorViewport> EditorViewports;

	FGizmo Gizmo;
	FGrid Grid;
	UObject* SelectedObject = nullptr;
};
