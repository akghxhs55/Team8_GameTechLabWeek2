#include "FImguiEditorViewportWindow.h"

#include "Runtime/CoreUObject/UPrimitiveComponent.h"
#include "Runtime/Engine/FRayCastingManager.h"
#include "Runtime/Math/FVector.h"

#include "ThirdParty/Imgui/imgui.h"
#include "ThirdParty/Imgui/imgui_internal.h"

// 뷰포트를 덮는 투명한 창
void FImguiEditorViewportWindow::Process(FEditor& Editor)
{
	const ImGuiViewport* MainViewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(MainViewport->WorkPos);
	ImGui::SetNextWindowSize(MainViewport->WorkSize);

	constexpr ImGuiWindowFlags WindowFlags =
		ImGuiWindowFlags_NoTitleBar |
		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoScrollbar |
		ImGuiWindowFlags_NoScrollWithMouse |
		ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_NoBackground |
		ImGuiWindowFlags_NoBringToFrontOnFocus |
		ImGuiWindowFlags_NoNavFocus;

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
	ImGui::Begin("##EditorViewport", nullptr, WindowFlags);
	ImGui::PopStyleVar(2);

	const ImVec2 ViewportPos = ImGui::GetCursorScreenPos();
	ImVec2 ViewportSize = ImGui::GetContentRegionAvail();
	if (ViewportSize.x < 1.0f) ViewportSize.x = 1.0f;
	if (ViewportSize.y < 1.0f) ViewportSize.y = 1.0f;

	// 뷰포트 영역 전체를 덮는 클릭 판정용 아이템.
	// 다른 ImGui 창이 위에 있으면 IsItemHovered()/IsItemClicked() 가 false 가 되어
	// 자연스럽게 focus 중재가 된다.
	ImGui::InvisibleButton("##ViewportInput", ViewportSize,
		ImGuiButtonFlags_MouseButtonLeft | ImGuiButtonFlags_MouseButtonRight);

	const bool bHovered = ImGui::IsItemHovered();
	const bool bFocused = ImGui::IsWindowFocused();

	const bool bPickRequested = ImGui::IsItemClicked(ImGuiMouseButton_Left);

	if (FEditorViewport* ActiveViewport = Editor.GetActiveViewport())
	{
		ActiveViewport->TopLeft = { ViewportPos.x, ViewportPos.y };
		ActiveViewport->Length = { ViewportSize.x, ViewportSize.y };
		ActiveViewport->UpdateFocusedAndHovered(bFocused, bHovered);

		if (bPickRequested)
		{
			HandlePicking(Editor, *ActiveViewport,
				FVector2{ ViewportSize.x, ViewportSize.y });
		}
	}

	ImGui::End();
}

void FImguiEditorViewportWindow::HandlePicking(
	FEditor& Editor, FEditorViewport& Viewport, FVector2 ViewportSize)
{
	TArray<UPrimitiveComponent*> Components = Editor.GetPrimitiveComponents();

	UPrimitiveComponent* HitComponent = nullptr;
	FVector ImpactPoint;

	// TODO: 뷰포트가 화면 전체가 아니게 되면(예: 도킹된 하위 창),
	//       FRayCastingManager 가 뷰포트 로컬 마우스 좌표
	//       (FInputManager 좌표 - Viewport.TopLeft) 를 인수로 받도록
	//       시그니처를 바꿔야 한다. 지금은 뷰포트 == 클라이언트 영역이라 그대로 사용.
	const bool bHit = FRayCastingManager::Get().RayIntersectsMeshes(
		&Viewport.ViewportCamera,
		Components,
		HitComponent,
		ImpactPoint,
		ViewportSize);

	if (bHit)
	{
		Editor.SelectObject(HitComponent);
	}
	else
	{
		Editor.UnSelectObject();
	}
}
