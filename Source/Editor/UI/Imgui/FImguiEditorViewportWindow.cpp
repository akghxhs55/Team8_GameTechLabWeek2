#include "FImguiEditorViewportWindow.h"

#include "Runtime/CoreUObject/UPrimitiveComponent.h"
#include "Runtime/Engine/FRayCastingManager.h"
#include "Runtime/Input/FInputManager.h"
#include "Runtime/Math/FVector.h"
#include "ThirdParty/Imgui/imgui.h"

// 뷰포트를 덮는 투명한 창
void FImguiEditorViewportWindow::Process(FEditor& Editor,float DeltaTime)
{
	const ImGuiViewport* MainViewport = ImGui::GetMainViewport();
	const FEditorViewport* Viewport = Editor.GetActiveViewport();
	if (!Viewport)
		return;

	const FVector2 ClientSize = { MainViewport->Size.x, MainViewport->Size.y };
	const FVector2 ViewportTopLeftPixels = Viewport->TopLeftUV * ClientSize;
	const FVector2 ViewportSizePixels = Viewport->LengthUV * ClientSize;

	ImGui::SetNextWindowPos(ImVec2(MainViewport->Pos.x + ViewportTopLeftPixels.X,
		MainViewport->Pos.y + ViewportTopLeftPixels.Y));
	ImGui::SetNextWindowSize(ImVec2(ViewportSizePixels.X, ViewportSizePixels.Y));

	constexpr ImGuiWindowFlags WindowFlags =
		ImGuiWindowFlags_NoTitleBar |
		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoScrollbar |
		ImGuiWindowFlags_NoScrollWithMouse |
		ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_NoBackground |
		ImGuiWindowFlags_NoSavedSettings |
		ImGuiWindowFlags_NoBringToFrontOnFocus |
		ImGuiWindowFlags_NoNavFocus;

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowMinSize, ImVec2(1.0f, 1.0f));
	ImGui::Begin("##EditorViewport", nullptr, WindowFlags);
	ImGui::PopStyleVar(3);

	// 뷰포트 영역 전체를 덮는 클릭 판정용 아이템.
	// 다른 ImGui 창이 위에 있으면 IsItemHovered()/IsItemClicked() 가 false 가 되어
	// 자연스럽게 focus 중재가 된다.
	ImGui::InvisibleButton("##ViewportInput", ImVec2(ViewportSizePixels.X, ViewportSizePixels.Y),
		ImGuiButtonFlags_MouseButtonLeft | ImGuiButtonFlags_MouseButtonRight);

	const bool bHovered = ImGui::IsItemHovered();
	const bool bFocused = ImGui::IsWindowFocused();

	const bool bPickRequested = ImGui::IsItemClicked(ImGuiMouseButton_Left);
	const bool bLeftDown = ImGui::IsMouseDown(ImGuiMouseButton_Left);
	const bool bLeftReleased = ImGui::IsMouseReleased(ImGuiMouseButton_Left);

	if (FEditorViewport* ActiveViewport = Editor.GetActiveViewport())
	{
		ActiveViewport->UpdateFocusedAndHovered(bFocused, bHovered);

		const FVector2 LocalMouse = FInputManager::Get().GetMousePosition() - ViewportTopLeftPixels;
	
		FGizmo& Gizmo = Editor.GetGizmo();

		if (bPickRequested)
		{
			HandlePicking(Editor, *ActiveViewport, LocalMouse, ViewportSizePixels);
		}

		if (bLeftDown)
		{
			Gizmo.UpdateInteraction(Editor, LocalMouse);
		}

		if (bLeftReleased)
		{
			Gizmo.EndInteraction();
		}

		if (bHovered)
		{
			UpdateGizmoHover(Editor, *ActiveViewport, LocalMouse, ViewportSizePixels);
		}
		else
		{
			Gizmo.HoveredHandle = EGizmoHandle::None;
		}

		if (bFocused)
		{
			FCamera& Camera = ActiveViewport->ViewportCamera;
			CameraController.UpdateMouseInput(Camera);
			if (FInputManager::Get().IsMouseDown(EMouseButton::Right))
			{
				CameraController.UpdateKeyInput(Camera, DeltaTime);
			}
			else
			{
				if (!Gizmo.IsInteracting())
				{
					if (FInputManager::Get().IsKeyJustPressed(VK_OEM_3)) // 백틱 (`)
					{
						if (Gizmo.Mode != EGizmoMode::None && Gizmo.Mode != EGizmoMode::Scale)
						{
							Gizmo.SetGizmoSpace(static_cast<EGizmoSpace>((static_cast<uint8>(Gizmo.GetSpace()) + 1) % 2));
						}
					}
					if (FInputManager::Get().IsKeyJustPressed('Q'))
					{
						Gizmo.Mode = EGizmoMode::None;
					}
					else if (FInputManager::Get().IsKeyJustPressed('W'))
					{
						Gizmo.Mode = EGizmoMode::Translate;
					}
					else if (FInputManager::Get().IsKeyJustPressed('E'))
					{
						Gizmo.Mode = EGizmoMode::Rotate;
					}
					else if (FInputManager::Get().IsKeyJustPressed('R'))
					{
						Gizmo.Mode = EGizmoMode::Scale;
					}
					else if (FInputManager::Get().IsKeyJustPressed(VK_SPACE))
					{
						Gizmo.Mode = static_cast<EGizmoMode>((static_cast<uint8>(Gizmo.Mode) + 1) % 4);
					}
				}
			}
		}
	}

	ImGui::End();
}

void FImguiEditorViewportWindow::HandlePicking(
	FEditor& Editor, const FEditorViewport& Viewport,
	const FVector2& LocalMousePixels, const FVector2& ViewportSizePixels)
{
	FGizmo& Gizmo = Editor.GetGizmo();
	if (Gizmo.HoveredHandle != EGizmoHandle::None)
	{
		Gizmo.BeginInteraction(Editor, Gizmo.HoveredHandle, LocalMousePixels, Viewport.ViewportCamera, ViewportSizePixels);
		return;
	}

	TArray<UPrimitiveComponent*> Components = Editor.GetPrimitiveComponents();

	UPrimitiveComponent* HitComponent = nullptr;
	FVector ImpactPoint;

//		TODO: 뷰포트가 화면 전체가 아니게 되면(예 : 도킹된 하위 창),
//       FRayCastingManager 가 뷰포트 로컬 마우스 좌표
//       (FInputManager 좌표 - Viewport.ScreenSpaceTopLeft) 를 인수로 받도록
//       시그니처를 바꿔야 한다. 지금은 뷰포트 == 클라이언트 영역이라 그대로 사용. ==> DONE
	const bool bHit = FRayCastingManager::RayIntersectsMeshes(
		FRayCastingManager::CreateRayFromScreenPosition(Viewport.ViewportCamera, LocalMousePixels, ViewportSizePixels),
		Components,
		HitComponent,
		ImpactPoint);

	if (bHit)
	{
		Editor.SelectObject(HitComponent);
	}
	else
	{
		Editor.UnSelectObject();
	}
}

void FImguiEditorViewportWindow::UpdateGizmoHover(FEditor& Editor, const FEditorViewport& Viewport,
	const FVector2& LocalMousePixels, const FVector2& ViewportSizePixels)
{
	FRay Ray = FRayCastingManager::CreateRayFromScreenPosition(
		Viewport.ViewportCamera, LocalMousePixels, ViewportSizePixels);
	
	FGizmo& Gizmo = Editor.GetGizmo();
	Gizmo.HoveredHandle = Gizmo.HitTest(Editor, Ray, Viewport.ViewportCamera);
}
