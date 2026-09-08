#include "FEditor.h"
#include "Runtime/Rendering/FRenderResourceLibrary.h"
#include "Runtime/CoreUObject/UObjectGlobals.h"
#include "Runtime/CoreUObject/UCubeComp.h"
#include "Runtime/CoreUObject/UCylinderComp.h"

void FEditor::Initialize(FRenderResourceLibrary* RendererLibrary, USceneManager* SceneManager)
{
    this->RendererLibrary = RendererLibrary;
    this->SceneManager = SceneManager;
}

void FEditor::NewScene()
{
    SelectedObject = nullptr;
    SceneManager->SetScene(NewObject<UScene>(*RendererLibrary));
}

void FEditor::SaveScene(const FString& path)
{
    SceneManager->SaveScene(path);
}

void FEditor::LoadScene(const FString& path) // TODO: 테스트용 임시 코드. 정식 코드로 교체해야 함
{
    // TODO: 이전 씬과 내부 오브젝트들은 GUObject의 가비지 컬렉션에 의해 삭제됨(구현 필요-현재 메모리 누수되고있음)
    SceneManager->currentScene = NewObject<UScene>(*RendererLibrary);
    UCubeComp* TestCube = NewObject<UCubeComp>();
    TestCube->RelativeTransform.Location = FVector{ 2.0f, 0.0f, 0.0f };
    TestCube->RelativeTransform.Rotation = FQuaternion::FromEulerXYZDeg(FVector{ 0.0f, 0.0f, 0.0f });
    TestCube->RelativeTransform.Scale3D = FVector{ 0.5f, 0.5f, 0.5f };
    SceneManager->currentScene->RegisterComponent(*TestCube);
    SelectedObject = TestCube;
    SceneManager->LoadScene(path);
    SelectedObject = nullptr;
}

bool FEditor::CheckSceneExists()
{
    if (SceneManager->currentScene == nullptr)
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
    return true;
}

void FEditor::UnSelectObject()
{
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
    if (!SceneManager || !SceneManager->currentScene)
    {
        return {};
    }
    return SceneManager->currentScene->GetPrimitiveComponents();
}

UPrimitiveComponent* FEditor::SpawnPrimitive(EEditorPrimitiveType Type)
{
    if (!SceneManager || !SceneManager->currentScene)
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

    SceneManager->currentScene->RegisterComponent(*Component);
    SelectedObject = Component;
    return Component;
}