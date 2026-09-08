#include "FEditor.h"

#include "Runtime/Rendering/FRenderResourceLibrary.h"
#include "Runtime/CoreUObject/UCubeComp.h"
#include "Runtime/CoreUObject/UCylinderComp.h"
#include <numbers>

void FEditor::Initialize(FRenderResourceLibrary* RendererLibrary, USceneManager* SceneManager)
{
	Gizmo.Initialize(*RendererLibrary);
	Grid.Initialize(*RendererLibrary);
	this->RendererLibrary = RendererLibrary;
	this->SceneManager = SceneManager;
}

void FEditor::Process()
{
	if (SelectedObject)
	{
		auto* SceneComp = dynamic_cast<USceneComponent*>(SelectedObject);
		if (SceneComp)
		{
			SceneComp->RelativeTransform = SelectedTransform;
		}
	}
}

void FEditor::NewScene()
{
	SelectedObject = nullptr;
	SceneManager->SetScene(NewObject<UScene>(*RendererLibrary));
}

void FEditor::SaveScene(const FString& Path)
{
	SceneManager->SaveScene(Path);
}

void FEditor::LoadScene(const FString& Path)
{
	// TODO: 이전 씬과 내부 오브젝트들은 GUObject의 가비지 컬렉션에 의해 삭제됨(구현 필요-현재 메모리 누수되고있음)
	//SceneManager->CurrentScene = NewObject<UScene>(*RendererLibrary);
	SceneManager->LoadScene(Path);
	SelectedObject = nullptr;
}

bool FEditor::CheckSceneExists()
{
	if (SceneManager->CurrentScene == nullptr)
		return false;
	return true;
}

void FEditor::AddViewport(FEditorViewport Viewport)
{
	EditorViewports.push_back(Viewport);
}

void FEditor::DeleteViewport(int32 IndexOfViewport)
{
	EditorViewports.erase(EditorViewports.begin() + IndexOfViewport);
}

FEditorViewport* FEditor::GetActiveViewport()
{
	if (EditorViewports.empty())
	{
		return nullptr;
	}
	return &EditorViewports[0];
}

bool FEditor::SelectObject(UObject* Object)
{
	if (Object == nullptr)
		return false;
	SelectedObject = Object;
	auto* SceneComp = dynamic_cast<USceneComponent*>(SelectedObject);
	if (SceneComp)
	{
		SelectedTransform = SceneComp->RelativeTransform;
	}

	return true;
}

void FEditor::UnSelectObject()
{
	if (SelectedObject)
	{
		auto* SceneComp = dynamic_cast<USceneComponent*>(SelectedObject);
		if (SceneComp)
		{
			SceneComp->RelativeTransform = SelectedTransform;
		}
	}
	SelectedObject = nullptr;
}

UObject* FEditor::GetSelectedObject()
{
	return SelectedObject;
}

const TArray<FEditorViewport>& FEditor::GetViewports() const
{
	return EditorViewports;
}

TArray<UPrimitiveComponent*> FEditor::GetPrimitiveComponents() const
{
	if (!SceneManager || !SceneManager->CurrentScene)
	{
		return {};
	}
	return SceneManager->CurrentScene->GetPrimitiveComponents();
}

UPrimitiveComponent* FEditor::SpawnPrimitive(EEditorPrimitiveType Type)
{
	if (!SceneManager || !SceneManager->CurrentScene)
	{
		return nullptr;
	}

	UPrimitiveComponent* Component = nullptr;
	switch (Type)
	{
	case EEditorPrimitiveType::Cube:
		Component = NewObject<UCubeComp>();
		break;
	case EEditorPrimitiveType::Cylinder:
		Component = NewObject<UCylinderComp>();
		break;
	case EEditorPrimitiveType::Sphere:
		// TODO: USphereComp + 스피어 메시가 없어 아직 생성 불가
		return nullptr;
	}

	if (!Component)
	{
		return nullptr;
	}

	// 완전히 겹치지 않게 살짝 오프셋 (임시)
	static int SpawnSerial = 0;
	const float Offset = 0.25f * static_cast<float>(SpawnSerial++);
	Component->RelativeTransform.Location = FVector{ Offset, 0.0f, 0.0f };
	Component->RelativeTransform.Scale3D = FVector{ 0.5f, 0.5f, 0.5f };

	SceneManager->CurrentScene->RegisterComponent(*Component);
	SelectedObject = Component;
	return Component;
}
