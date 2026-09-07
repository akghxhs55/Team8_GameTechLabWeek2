#include "FImguiPropertyWindow.h"
#include "Runtime/CoreUObject/USceneComponent.h"
#include "ThirdParty/Imgui/imgui.h"
#include "ThirdParty/Imgui/imgui_internal.h"
#include "ThirdParty/Imgui/imgui_impl_dx11.h"
#include "ThirdParty/Imgui/imgui_impl_win32.h"

void FImguiPropertyWindow::Process(FEditor& Editor)
{
	// TODO: RTTI 구현시 dynamic_cast 대신 자체 방법으로 실행되어야 함
	auto* SceneComponent = dynamic_cast<USceneComponent*>(Editor.GetSelectedObject());
	ImGui::Begin("Jungle Property Window");

	if (SceneComponent) {
		FTransform RelativeTransform = SceneComponent->GetRelativeTransform();
		float Location[3] = { RelativeTransform.Location[0], RelativeTransform.Location[1], RelativeTransform.Location[2] };

		ImGui::DragFloat3("Translation", Location);
		RelativeTransform.Location = { Location[0], Location[1], Location[2] };
		SceneComponent->SetRelativeTransform(RelativeTransform);
		//ImGui::DragFloat3("Rotation", Rotation);
		//ImGui::DragFloat3("Scale", Scale);
	}
	ImGui::End();
}
