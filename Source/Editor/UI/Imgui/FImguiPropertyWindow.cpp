#include "FImguiPropertyWindow.h"
#include "Runtime/CoreUObject/USceneComponent.h"

void FImguiPropertyWindow::Process(FEditor& Editor)
{
	auto* SceneComponent = dynamic_cast<USceneComponent*>(Editor.GetSelectedObject());
	ImGui::Begin("Jungle Property Window");

	if (SceneComponent) {
		FTransform RelativeTransform = SceneComponent->GetRelativeTransform();
		float Location[3] = { RelativeTransform.Location[0], RelativeTransform.Location[1], RelativeTransform.Location[2] };

		//ImGui::DragFloat3("Translation", RelativeTransform.Location);
		//ImGui::DragFloat3("Rotation", Rotation);
		//ImGui::DragFloat3("Scale", Scale);
	}
	ImGui::End();
}
